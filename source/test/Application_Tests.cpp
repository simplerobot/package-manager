#include "Test.hpp"
#include "Application.hpp"
#include "ApplicationPartialMock.hpp"
#include "SystemMock.hpp"


TEST_CASE(Application_Constructor)
{
	SystemMock system_mock;
	Application test(system_mock);
}

TEST_CASE(Application_PrintHelp)
{
	SystemMock system_mock;
	Application test(system_mock);

	test.PrintHelp("a.out");
}

TEST_CASE(Application_Run_ImplicitLoad)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFetchDependencies(EXITCODE_SUCCESS);

	int result = test.Run({ "srpm", "test_package_filename.srpm" });

	ASSERT(result == EXITCODE_SUCCESS);
}

TEST_CASE(Application_Run_Help)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("test-app-name");

	int result = test.Run({ "test-app-name", "--help" });

	ASSERT(result == EXITCODE_SUCCESS);
}

TEST_CASE(Application_Run_ExplicitLoad)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFetchDependencies(EXITCODE_SUCCESS);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--load" });

	ASSERT(result == EXITCODE_SUCCESS);
}

TEST_CASE(Application_Run_FlushAll)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFlushAllDependencies(EXITCODE_SUCCESS);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--flush-all" });

	ASSERT(result == EXITCODE_SUCCESS);
}

TEST_CASE(Application_Run_Flush)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFlushDependencies({ "a", "b" }, EXITCODE_SUCCESS);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--flush", "a", "--flush", "b" });

	ASSERT(result == EXITCODE_SUCCESS);
}

TEST_CASE(Application_Run_InvalidArgument)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("a.out");

	int result = test.Run({ "a.out", "test_package_filename.srpm", "--invalid-param" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_MultiplePackageFiles)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("a.out");

	int result = test.Run({ "a.out", "test_package_filename.srpm", "test_package_filename-b.srpm" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_MultipleCommands)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("a.out");

	int result = test.Run({ "a.out", "test_package_filename.srpm", "--load", "--help" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_FlushAllAndFlush)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("a.out");

	int result = test.Run({ "a.out", "test_package_filename.srpm", "--flush-all", "--flush", "a" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_HelpWithPackage)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("test-app-name");

	int result = test.Run({ "test-app-name", "pkg.srpm", "--help" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_LoadNoPackage)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("test-app-name");

	int result = test.Run({ "test-app-name", "--load" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_FlushEmpty)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("test-app-name");

	int result = test.Run({ "test-app-name", "pkg.srpm", "--flush" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_FlushNoPackage)
{
	ApplicationPartialMock test;
	test.ExpectPrintHelp("test-app-name");

	int result = test.Run({ "test-app-name", "--flush", "a" });

	ASSERT(result == EXITCODE_INVALID_ARGUMENTS);
}

TEST_CASE(Application_Run_LoadFailed)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFetchDependencies(12345);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--load" });

	ASSERT(result == 12345);
}

TEST_CASE(Application_Run_FlushAllFailed)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFlushAllDependencies(12345);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--flush-all" });

	ASSERT(result == 12345);
}

TEST_CASE(Application_Run_FlushFailed)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), true);
	test.ExpectFlushDependencies({ "a", "b" }, 12345);

	int result = test.Run({ "srpm", "test_package_filename.srpm", "--flush", "a", "--flush", "b" });

	ASSERT(result == 12345);
}

TEST_CASE(Application_Run_LoadConfigFailed)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), false);

	int result = test.Run({ "srpm", "test_package_filename.srpm" });

	ASSERT(result == EXITCODE_INVALID_CONFIGURATION);
}

TEST_CASE(Application_Run_LoadPackageFailed)
{
	ApplicationPartialMock test;
	test.ExpectLoadApplicationConfig(ApplicationConfig(), true);
	test.ExpectLoadPackageConfig("test_package_filename.srpm", PackageConfig(), false);

	int result = test.Run({ "srpm", "test_package_filename.srpm" });

	ASSERT(result == EXITCODE_INVALID_PACKAGE);
}

TEST_CASE(Application_LoadApplicationConfig_HappyCase)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetConfigurationDir("/non/existant/dir");
	system_mock.SetFile("/non/existant/dir/.srpm/config", R"({ "cache-dir": "~/test/cache/dir/", "cache-valid-seconds": 1234567, "client-cert": "~/test-client-cert.pem", "client-key": "~/test-client-key.pem", })");

	ApplicationConfig application_config;
	ASSERT(test.LoadApplicationConfig(application_config));

	ASSERT(application_config.GetCacheDir() == "~/test/cache/dir/");
	ASSERT(application_config.GetCacheValidTimeS() == 1234567);
	ASSERT(application_config.GetClientCertFile() == "~/test-client-cert.pem");
	ASSERT(application_config.GetClientKeyFile() == "~/test-client-key.pem");
}

TEST_CASE(Application_LoadApplicationConfig_MissingFile)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetConfigurationDir("/non/existant/dir");

	ApplicationConfig application_config;
	ASSERT(test.LoadApplicationConfig(application_config));
}

TEST_CASE(Application_LoadApplicationConfig_ParseFailed)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetConfigurationDir("/non/existant/dir");
	system_mock.SetFile("/non/existant/dir/.srpm/config", "gibberish");

	ApplicationConfig application_config;
	ASSERT(!test.LoadApplicationConfig(application_config));
}

TEST_CASE(Application_LoadApplicationConfig_LoadFailed)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetConfigurationDir("/non/existant/dir");
	system_mock.SetFile("/non/existant/dir/.srpm/config", "12345");

	ApplicationConfig application_config;
	ASSERT(!test.LoadApplicationConfig(application_config));
}

TEST_CASE(Application_LoadPackageConfig_HappyCase)
{
	SystemMock system_mock;
	Application test(system_mock);
	const char* PACKAGE_FILE_CONTENTS = R"(
		{
		  "name": "package-manager",
		  "config-url": "https://github.com/simplerobot/package-manager/releases/latest/download/package.srpm",
		  "binary-url": "https://github.com/simplerobot/package-manager/releases/latest/download/package.tgz",
		  "release-dir": "build/release",
	
		  "dependencies":
		  [
			{
			  "name": "jsoncpp",
			  "config-url": "https://github.com/simplerobot/package-config/parsers/json/jsoncpp/jsoncpp.srpm",
			  "import-dir": "build/dependencies/jsoncpp",
			},
			{
			  "name": "libcurl",
			  "config-url": "https://github.com/simplerobot/package-config/network-clients/libcurl/libcurl.srpm",
			  "import-dir": "build/dependencies/libcurl", 
			},
		  ],
		}
	)";
	system_mock.SetFile("directory/file.srpm", PACKAGE_FILE_CONTENTS);

	PackageConfig package_config;
	ASSERT(test.LoadPackageConfig("directory/file.srpm", package_config));

	ASSERT(package_config.GetName() == "package-manager");
	ASSERT(package_config.GetConfigUrl() == "https://github.com/simplerobot/package-manager/releases/latest/download/package.srpm");
	ASSERT(package_config.GetBinaryUrl() == "https://github.com/simplerobot/package-manager/releases/latest/download/package.tgz");
	ASSERT(package_config.GetReleaseDir() == "build/release");
	ASSERT(!package_config.HasImportDir());
	ASSERT(package_config.begin() != package_config.end());
	ASSERT(package_config.begin()->GetName() == "jsoncpp");
}

TEST_CASE(Application_LoadPackageConfig_MissingFile)
{
	SystemMock system_mock;
	Application test(system_mock);

	PackageConfig package_config;
	ASSERT(!test.LoadPackageConfig("directory/file.srpm", package_config));
}

TEST_CASE(Application_LoadPackageConfig_ParseFailed)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetFile("directory/file.srpm", "blah, blah, back sheep");

	PackageConfig package_config;
	ASSERT(!test.LoadPackageConfig("directory/file.srpm", package_config));
}

TEST_CASE(Application_LoadPackageConfig_LoadFailed)
{
	SystemMock system_mock;
	Application test(system_mock);
	system_mock.SetFile("directory/file.srpm", "27");

	PackageConfig package_config;
	ASSERT(!test.LoadPackageConfig("directory/file.srpm", package_config));
}

/*
	virtual int FetchDependencies(const ApplicationConfig& config, const PackageConfig& package);
	virtual int FlushAllDependencies(const ApplicationConfig& config, const PackageConfig& package);
	virtual int FlushDependencies(const ApplicationConfig& config, const PackageConfig& package, const std::vector<std::string>& target_dependencies);
 */
