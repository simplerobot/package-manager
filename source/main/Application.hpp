#pragma once

#include "ApplicationConfig.hpp"
#include "Main.hpp"
#include "PackageConfig.hpp"
#include "SystemInterface.hpp"


constexpr int EXITCODE_SUCCESS = 0;
constexpr int EXITCODE_INVALID_ARGUMENTS = 1;
constexpr int EXITCODE_INVALID_CONFIGURATION = 2;
constexpr int EXITCODE_INVALID_PACKAGE = 3;


class Application
{
public:
	Application(SystemInterface& system);
	virtual ~Application();

	int Run(int argc, char* const argv[]);

	virtual bool LoadApplicationConfig(ApplicationConfig& application_config);
	virtual bool LoadPackageConfig(const char* package_filename, PackageConfig& package_config);

	virtual void PrintHelp(const char* application_name);
	virtual int LoadDependencies(const ApplicationConfig& config, const PackageConfig& package);

private:
	SystemInterface& m_system;
};
