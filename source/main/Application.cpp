#include "Application.hpp"
#include <getopt.h>


Application::Application(SystemInterface& system)
	: m_system(system)
{
}

Application::~Application()
{
}

void Application::PrintHelp(const char* application_name)
{
	std::printf("%s --help                       Prints help\n", application_name);
	std::printf("%s [<package-file>] [--load]    Loads dependencies\n", application_name);
}

int Application::Run(int argc, char* const argv[])
{
	static struct option long_options[] = {
		{ "help",         no_argument,       nullptr, 'h' },
		{ "load",         no_argument,       nullptr, 'l' },
		{ nullptr, 0, nullptr, 0 }
	};

	bool error = false;
	bool command_show_help = false;
	bool command_load = false;
	const char* package_file = nullptr;

	int option_index = 0;
	int c;
	optind = 1; // Make sure scanning starts at the beginning.
	while ((c = ::getopt_long(argc, argv, "", long_options, &option_index)) != -1)
	{
		switch (c)
		{
		case 'h': // help
			command_show_help = true;
			break;
		case 'l': // load
			command_load = true;
			break;
		case '?':
		default:
			error = true;
			std::printf("Error: Unknown parameter.\n");
			break;
		}
	}

	if (optind < argc)
	{
		package_file = argv[optind++];
	}

	for (int i = optind; i < argc; i++)
	{
		error = true;
		std::printf("Error: Unknown parameter '%s'\n", argv[i]);
	}

	// Default to load command.
	if (!command_show_help)
	{
		command_load = true;
	}

	// Check commands.
	if (command_show_help && command_load)
	{
		error = true;
		std::printf("Error: Multiple commands.\n");
	}
	if (command_show_help && package_file != nullptr)
	{
		error = true;
		std::printf("Error: Help command does not support a package file.\n");
	}
	if (command_load && package_file == nullptr)
	{
		error = true;
		std::printf("Error: Load command requires a package file.\n");
	}

	// Process command

	if (error)
	{
		PrintHelp(argv[0]);
		return EXITCODE_INVALID_ARGUMENTS;
	}

	if (command_show_help)
	{
		PrintHelp(argv[0]);
		return EXITCODE_SUCCESS;
	}

	if (command_load)
	{
		ApplicationConfig application_config;
		if (!LoadApplicationConfig(application_config))
		{
			std::printf("Error: Unable to load application configuration.\n");
			return EXITCODE_INVALID_CONFIGURATION;
		}

		PackageConfig package_config;
		if (package_file == nullptr)
			package_file = "package.srpm";
		if (!LoadPackageConfig(package_file, package_config))
		{
			std::printf("Error: Unable to load package configuration file '%s'.\n", package_file);
			return EXITCODE_INVALID_PACKAGE;
		}

		return LoadDependencies(application_config, package_config);
	}

	throw std::runtime_error("internal error");
}

bool Application::LoadApplicationConfig(ApplicationConfig& application_config)
{
	std::string config_dir = m_system.GetConfigurationDir();
	std::string config_filename = config_dir + "/.srpm/config";
	std::string config_text;
	if (!m_system.Load(config_filename, config_text))
	{
		return true;
	}

	Json::CharReaderBuilder builder;
	std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	std::string errors;
	Json::Value config_json;
	if (!reader->parse(config_text.data(), config_text.data() + config_text.size(), &config_json, &errors))
	{
		std::printf("Warning: Configuration file parse error '%s'.\n", errors.c_str());
		return false;
	}

	if (!application_config.Load(config_json))
	{
		std::printf("Warning: Invalid configuration data.\n");
		return false;
	}

	return true;
}

bool Application::LoadPackageConfig(const char* package_filename, PackageConfig& package_config)
{
	std::string package_text;
	if (!m_system.Load(package_filename, package_text))
	{
		std::printf("Warning: Unable to open file '%s'.\n", package_filename);
		return false;
	}

	Json::CharReaderBuilder builder;
	std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	std::string errors;
	Json::Value package_json;
	if (!reader->parse(package_text.data(), package_text.data() + package_text.size(), &package_json, &errors))
	{
		std::printf("Warning: Package file '%s' parse error '%s'.\n", package_filename, errors.c_str());
		return false;
	}

	if (!package_config.Load(package_json))
	{
		std::printf("Warning Invalid package data in file '%s'.\n", package_filename);
		return false;
	}

	return true;
}

int Application::LoadDependencies(const ApplicationConfig& config, const PackageConfig& package)
{
	throw std::runtime_error("not implemented");
}
