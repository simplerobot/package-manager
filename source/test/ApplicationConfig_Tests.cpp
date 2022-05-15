#include "Test.hpp"
#include "ApplicationConfig.hpp"


TEST_CASE(ApplicationConfig_Constructor)
{
	ApplicationConfig test;
}

TEST_CASE(ApplicationConfig_HasGetSet_CacheDir)
{
	ApplicationConfig test;

	ASSERT(!test.HasCacheDir());
	ASSERT_THROWS(test.GetCacheDir());
	ASSERT(test.GetCacheDir("abcd") == "abcd");

	test.SetCacheDir("test-cache-dir");

	ASSERT(test.HasCacheDir());
	ASSERT(test.GetCacheDir() == "test-cache-dir");
	ASSERT(test.GetCacheDir("abcd") == "test-cache-dir");
}

TEST_CASE(ApplicationConfig_HasGetSet_CacheValidTimeS)
{
	ApplicationConfig test;

	ASSERT(!test.HasCacheValidTimeS());
	ASSERT_THROWS(test.GetCacheValidTimeS());
	ASSERT(test.GetCacheValidTimeS(12345) == 12345);

	test.SetCacheValidTimeS(2357);

	ASSERT(test.HasCacheValidTimeS());
	ASSERT(test.GetCacheValidTimeS() == 2357);
	ASSERT(test.GetCacheValidTimeS(12345) == 2357);
}

TEST_CASE(ApplicationConfig_HasGetSet_ClientCertFile)
{
	ApplicationConfig test;

	ASSERT(!test.HasClientCertFile());
	ASSERT_THROWS(test.GetClientCertFile());

	test.SetClientCertFile("test-client-cert-file");

	ASSERT(test.HasClientCertFile());
	ASSERT(test.GetClientCertFile() == "test-client-cert-file");
}

TEST_CASE(ApplicationConfig_HasGetSet_ClientKeyFile)
{
	ApplicationConfig test;

	ASSERT(!test.HasClientKeyFile());
	ASSERT_THROWS(test.GetClientKeyFile());

	test.SetClientKeyFile("test-client-key-file");

	ASSERT(test.HasClientKeyFile());
	ASSERT(test.GetClientKeyFile() == "test-client-key-file");
}

TEST_CASE(ApplicationConfig_Load_HappyCase)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.Load(data));

	ASSERT(test.HasCacheDir() && test.GetCacheDir() == "test-cache-dir");
	ASSERT(test.HasCacheValidTimeS() && test.GetCacheValidTimeS() == 1234321);
	ASSERT(test.HasClientCertFile() && test.GetClientCertFile() == "test-client-cert");
	ASSERT(test.HasClientKeyFile() && test.GetClientKeyFile() == "test-client-key");
}

TEST_CASE(ApplicationConfig_Load_Empty)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);

	ASSERT(test.Load(data));

	ASSERT(!test.HasCacheDir());
	ASSERT(!test.HasCacheValidTimeS());
	ASSERT(!test.HasClientCertFile());
	ASSERT(!test.HasClientKeyFile());
}

TEST_CASE(ApplicationConfig_Load_InvalidFileType)
{
	ApplicationConfig test;

	ASSERT(!test.Load(Json::Value(Json::nullValue)));
	ASSERT(!test.Load(Json::Value(Json::stringValue)));
}

TEST_CASE(ApplicationConfig_Load_InvalidCacheDir)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = 2468;
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.Load(data));

	ASSERT(!test.HasCacheDir());
}

TEST_CASE(ApplicationConfig_Load_InvalidCacheValidSeconds)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = "invalid-time";
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.Load(data));

	ASSERT(!test.HasCacheValidTimeS());
}

TEST_CASE(ApplicationConfig_Load_InvalidCacheClientCert)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = 2468;
	data["client-key"] = "test-client-key";

	ASSERT(test.Load(data));

	ASSERT(!test.HasClientCertFile());
}

TEST_CASE(ApplicationConfig_Load_InvalidCacheClientKey)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = 2468;

	ASSERT(test.Load(data));

	ASSERT(!test.HasClientKeyFile());
}

