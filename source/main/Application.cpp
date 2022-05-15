#include "Application.hpp"


Application::Application(SystemInterface& system)
	: m_system(system)
{
}

Application::~Application()
{
}

int Application::Run(int argc, char* const argv[])
{
	throw std::runtime_error("not implemented");
	return EXITCODE_SUCCESS;
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
		return true;

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

void Application::PrintHelp(const char* application_name)
{
	std::printf("%s --help                                Prints help\n", application_name);
	std::printf("%s <project-file> [--load]               Loads dependencies\n", application_name);
	std::printf("%s <project-file> --flush-all            Flushes all cached dependencies\n", application_name);
	std::printf("%s <project-file> --flush <dependency>   Flushes a specific dependency\n", application_name);
}

int Application::FetchDependencies(const ApplicationConfig& config, const PackageConfig& package)
{
	throw std::runtime_error("not implemented");
}

int Application::FlushDependencies(const ApplicationConfig& config, const PackageConfig& package, const std::vector<std::string>& target_dependencies)
{
	throw std::runtime_error("not implemented");
}
