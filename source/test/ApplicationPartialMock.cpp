#include "ApplicationPartialMock.hpp"
#include "Test.hpp"


ApplicationPartialMock::ApplicationPartialMock()
	: Application(m_system_mock)
{
}

ApplicationPartialMock::~ApplicationPartialMock()
{
	if (!m_expected.empty())
		printf("Missing Expected: '%s'\n", m_expected.front().c_str());
	ASSERT(m_expected.empty());
}

int ApplicationPartialMock::Run(const std::vector<std::string>& args)
{
	int argc = args.size();
	std::vector<const char*> argv;
	for (auto& s : args)
		argv.push_back(s.c_str());
	return Application::Run(argc, (char* const*)argv.data());
}

bool ApplicationPartialMock::LoadApplicationConfig(ApplicationConfig& application_config)
{
	application_config = m_application_config;
	return Validate("LoadApplicationConfig") != 0;
}

bool ApplicationPartialMock::LoadPackageConfig(const char* package_filename, PackageConfig& package_config)
{
	package_config = m_package_config;
	std::stringstream out;
	out << "LoadPackageConfig " << package_filename;
	return Validate(out.str()) != 0;
}

void ApplicationPartialMock::PrintHelp(const char* application_name)
{
	std::stringstream out;
	out << "PrintHelp " << application_name;
	Validate(out.str());
}

int ApplicationPartialMock::LoadDependencies(const ApplicationConfig& config, const PackageConfig& package)
{
	return Validate("LoadDependencies");
}

void ApplicationPartialMock::ExpectLoadApplicationConfig(const ApplicationConfig& application_config, bool result)
{
	m_application_config = application_config;
	Expect("LoadApplicationConfig", result ? 1 : 0);
}

void ApplicationPartialMock::ExpectLoadPackageConfig(const char* package_filename, const PackageConfig& package_config, bool result)
{
	m_package_config = package_config;
	std::stringstream out;
	out << "LoadPackageConfig " << package_filename;
	Expect(out.str(), result ? 1 : 0);
}

void ApplicationPartialMock::ExpectPrintHelp(const char* application_name)
{
	std::stringstream out;
	out << "PrintHelp " << application_name;
	Expect(out.str(), 0);
}

void ApplicationPartialMock::ExpectLoadDependencies(int result)
{
	Expect("LoadDependencies", result);
}

void ApplicationPartialMock::Expect(const std::string& expected, int result)
{
	m_expected.push(expected);
	m_results.push(result);
}

int ApplicationPartialMock::Validate(const std::string& actual)
{
	if (m_expected.empty())
		printf("Unexpected call: '%s'\n", actual.c_str());
	ASSERT(!m_expected.empty());

	std::string expected = m_expected.front();
	m_expected.pop();

	int result = m_results.front();
	m_results.pop();

	if (expected != actual)
	{
		printf("Expected call '%s'\n", expected.c_str());
		printf("Actual call   '%s'\n", actual.c_str());
	}
	ASSERT(expected == actual);

	return result;
}
