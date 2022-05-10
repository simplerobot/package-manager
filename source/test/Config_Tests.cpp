#include "Test.hpp"
#include "Config.hpp"


TEST_CASE(Config_Constructor)
{
	Config test;
}

TEST_CASE(Config_HasGetSet_CacheDir)
{
	Config test;

	ASSERT(!test.HasCacheDir());
	ASSERT_THROWS(test.GetCacheDir());
	ASSERT(test.GetCacheDir("abcd") == "abcd");

	test.SetCacheDir("test-cache-dir");

	ASSERT(test.HasCacheDir());
	ASSERT(test.GetCacheDir() == "test-cache-dir");
	ASSERT(test.GetCacheDir("abcd") == "test-cache-dir");
}

TEST_CASE(Config_HasGetSet_CacheValidTimeS)
{
	Config test;

	ASSERT(!test.HasCacheValidTimeS());
	ASSERT_THROWS(test.GetCacheValidTimeS());
	ASSERT(test.GetCacheValidTimeS(12345) == 12345);

	test.SetCacheValidTimeS(2357);

	ASSERT(test.HasCacheValidTimeS());
	ASSERT(test.GetCacheValidTimeS() == 2357);
	ASSERT(test.GetCacheValidTimeS(12345) == 2357);
}

TEST_CASE(Config_HasGetSet_ClientCertFile)
{
	Config test;

	ASSERT(!test.HasClientCertFile());
	ASSERT_THROWS(test.GetClientCertFile());

	test.SetClientCertFile("test-client-cert-file");

	ASSERT(test.HasClientCertFile());
	ASSERT(test.GetClientCertFile() == "test-client-cert-file");
}

TEST_CASE(Config_HasGetSet_ClientKeyFile)
{
	Config test;

	ASSERT(!test.HasClientKeyFile());
	ASSERT_THROWS(test.GetClientKeyFile());

	test.SetClientKeyFile("test-client-key-file");

	ASSERT(test.HasClientKeyFile());
	ASSERT(test.GetClientKeyFile() == "test-client-key-file");
}

TEST_CASE(Config_LoadConfig_HappyCase)
{
	Config test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.LoadConfig(data));

	ASSERT(test.HasCacheDir() && test.GetCacheDir() == "test-cache-dir");
	ASSERT(test.HasCacheValidTimeS() && test.GetCacheValidTimeS() == 1234321);
	ASSERT(test.HasClientCertFile() && test.GetClientCertFile() == "test-client-cert");
	ASSERT(test.HasClientKeyFile() && test.GetClientKeyFile() == "test-client-key");
}

TEST_CASE(Config_LoadConfig_Empty)
{
	Config test;
	Json::Value data(Json::objectValue);

	ASSERT(test.LoadConfig(data));

	ASSERT(!test.HasCacheDir());
	ASSERT(!test.HasCacheValidTimeS());
	ASSERT(!test.HasClientCertFile());
	ASSERT(!test.HasClientKeyFile());
}

TEST_CASE(Config_LoadConfig_InvalidFileType)
{
	Config test;

	ASSERT(!test.LoadConfig(Json::Value(Json::nullValue)));
	ASSERT(!test.LoadConfig(Json::Value(Json::stringValue)));
}

TEST_CASE(Config_LoadConfig_InvalidCacheDir)
{
	Config test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = 2468;
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.LoadConfig(data));

	ASSERT(!test.HasCacheDir());
}

TEST_CASE(Config_LoadConfig_InvalidCacheValidSeconds)
{
	Config test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = "invalid-time";
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.LoadConfig(data));

	ASSERT(!test.HasCacheValidTimeS());
}

TEST_CASE(Config_LoadConfig_InvalidCacheClientCert)
{
	Config test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = 2468;
	data["client-key"] = "test-client-key";

	ASSERT(test.LoadConfig(data));

	ASSERT(!test.HasClientCertFile());
}

TEST_CASE(Config_LoadConfig_InvalidCacheClientKey)
{
	Config test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = 2468;

	ASSERT(test.LoadConfig(data));

	ASSERT(!test.HasClientKeyFile());
}

