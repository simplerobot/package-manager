#pragma once

#include "ApplicationConfig.hpp"
#include "Main.hpp"
#include "PackageConfig.hpp"
#include "SystemInterface.hpp"


constexpr int EXITCODE_SUCCESS = 0;


class Application
{
public:
	Application(SystemInterface& system);
	virtual ~Application();

	int Run(int argc, char* const argv[]);

	virtual bool LoadApplicationConfig(ApplicationConfig& application_config);
	virtual bool LoadPackageConfig(const char* package_filename, PackageConfig& package_config);

	virtual void PrintHelp(const char* application_name);
	virtual int FetchDependencies(const ApplicationConfig& config, const PackageConfig& package);
	virtual int FlushDependencies(const ApplicationConfig& config, const PackageConfig& package, const std::vector<std::string>& target_dependencies);

private:
	SystemInterface& m_system;
};
