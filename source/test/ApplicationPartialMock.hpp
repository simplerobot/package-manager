#pragma once

#include "Main.hpp"
#include "Application.hpp"
#include "SystemMock.hpp"
#include <queue>


class ApplicationPartialMock : public Application
{
public:
	ApplicationPartialMock();
	virtual ~ApplicationPartialMock();

	int Run(const std::vector<std::string>& args);

	virtual bool LoadApplicationConfig(ApplicationConfig& application_config) override;
	virtual bool LoadPackageConfig(const char* package_filename, PackageConfig& package_config) override;

	virtual void PrintHelp(const char* application_name) override;
	virtual int LoadDependencies(const ApplicationConfig& config, const PackageConfig& package) override;

	void ExpectLoadApplicationConfig(const ApplicationConfig& application_config, bool result);
	void ExpectLoadPackageConfig(const char* package_filename, const PackageConfig& package_config, bool result);

	void ExpectPrintHelp(const char* application_name);
	void ExpectLoadDependencies(int result);


private:
	SystemMock m_system_mock;

	void Expect(const std::string& expected, int result);
	int Validate(const std::string&);

	std::queue<std::string> m_expected;
	ApplicationConfig m_application_config;
	PackageConfig m_package_config;
	std::queue<int> m_results;
};


