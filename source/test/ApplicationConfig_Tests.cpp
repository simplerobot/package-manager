#include "Test.hpp"
#include "ApplicationConfig.hpp"


TEST_CASE(ApplicationServerConfig_Constructor)
{
	ApplicationServerConfig test;
}

TEST_CASE(ApplicationServerConfig_HasGetSet_Host)
{
	ApplicationServerConfig test;

	ASSERT(!test.HasHost());
	ASSERT_THROWS(test.GetHost());

	test.SetHost("test-host-name");

	ASSERT(test.HasHost());
	ASSERT(test.GetHost() == "test-host-name");
}

TEST_CASE(ApplicationServerConfig_HasGetSet_ClientCertFile)
{
	ApplicationServerConfig test;

	ASSERT(!test.HasClientCertFile());
	ASSERT_THROWS(test.GetClientCertFile());

	test.SetClientCertFile("test-client-cert-file");

	ASSERT(test.HasClientCertFile());
	ASSERT(test.GetClientCertFile() == "test-client-cert-file");
}

TEST_CASE(ApplicationServerConfig_HasGetSet_ClientKeyFile)
{
	ApplicationServerConfig test;

	ASSERT(!test.HasClientKeyFile());
	ASSERT_THROWS(test.GetClientKeyFile());

	test.SetClientKeyFile("test-client-key-file");

	ASSERT(test.HasClientKeyFile());
	ASSERT(test.GetClientKeyFile() == "test-client-key-file");
}

TEST_CASE(ApplicationServerConfig_Load_HappyCase)
{
	ApplicationServerConfig test;
	Json::Value data(Json::objectValue);
	data["host"] = "test-host-name";
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(test.Load(data));

	ASSERT(test.HasHost() && test.GetHost() == "test-host-name");
	ASSERT(test.HasClientCertFile() && test.GetClientCertFile() == "test-client-cert");
	ASSERT(test.HasClientKeyFile() && test.GetClientKeyFile() == "test-client-key");
}

TEST_CASE(ApplicationServerConfig_Load_Empty)
{
	ApplicationServerConfig test;
	Json::Value data(Json::objectValue);

	ASSERT(test.Load(data));

	ASSERT(!test.HasHost());
	ASSERT(!test.HasClientCertFile());
	ASSERT(!test.HasClientKeyFile());
}

TEST_CASE(ApplicationServerConfig_Load_NotObject)
{
	ApplicationServerConfig test;
	Json::Value data(Json::stringValue);

	ASSERT(!test.Load(data));
}

TEST_CASE(ApplicationServerConfig_Load_HostNotString)
{
	ApplicationServerConfig test;
	Json::Value data(Json::objectValue);
	data["host"] = 5;
	data["client-cert"] = "test-client-cert";
	data["client-key"] = "test-client-key";

	ASSERT(!test.Load(data));
}

TEST_CASE(ApplicationServerConfig_Load_ClientCertNotString)
{
	ApplicationServerConfig test;
	Json::Value data(Json::objectValue);
	data["host"] = "test-host-name";
	data["client-cert"] = 5;
	data["client-key"] = "test-client-key";

	ASSERT(!test.Load(data));
}

TEST_CASE(ApplicationServerConfig_Load_ClientKeyNotString)
{
	ApplicationServerConfig test;
	Json::Value data(Json::objectValue);
	data["host"] = "test-host-name";
	data["client-cert"] = "test-client-cert";
	data["client-key"] = 5;

	ASSERT(!test.Load(data));
}

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

TEST_CASE(ApplicationConfig_Load_HappyCase)
{
	ApplicationConfig test;
	Json::Value server(Json::objectValue);
	server["host"] = "test-host-name";
	server["client-cert"] = "test-client-cert";
	server["client-key"] = "test-client-key";
	Json::Value servers(Json::arrayValue);
	servers.append(server);
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["servers"] = servers;

	ASSERT(test.Load(data));

	ASSERT(test.HasCacheDir() && test.GetCacheDir() == "test-cache-dir");
	ASSERT(test.HasCacheValidTimeS() && test.GetCacheValidTimeS() == 1234321);
	ASSERT(test.HasServerConfig("test-host-name") && test.GetServerConfig("test-host-name").GetClientCertFile() == "test-client-cert");
	ASSERT(test.HasServerConfig("test-host-name") && test.GetServerConfig("test-host-name").GetClientKeyFile() == "test-client-key");
}

TEST_CASE(ApplicationConfig_Load_Empty)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);

	ASSERT(test.Load(data));

	ASSERT(!test.HasCacheDir());
	ASSERT(!test.HasCacheValidTimeS());
	ASSERT(test.GetServerConfigCount() == 0);
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

	ASSERT(!test.Load(data));

	ASSERT(!test.HasCacheDir());
}

TEST_CASE(ApplicationConfig_Load_InvalidCacheValidSeconds)
{
	ApplicationConfig test;
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = "invalid-time";

	ASSERT(!test.Load(data));

	ASSERT(!test.HasCacheValidTimeS());
}

TEST_CASE(ApplicationConfig_Load_InvalidServer)
{
	ApplicationConfig test;
	Json::Value server(Json::objectValue);
	server["host"] = 5;
	server["client-cert"] = "test-client-cert";
	server["client-key"] = "test-client-key";
	Json::Value servers(Json::arrayValue);
	servers.append(server);
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["servers"] = servers;

	ASSERT(!test.Load(data));
}

TEST_CASE(ApplicationConfig_Load_MissingHostname)
{
	ApplicationConfig test;
	Json::Value server(Json::objectValue);
	server["client-cert"] = "test-client-cert";
	server["client-key"] = "test-client-key";
	Json::Value servers(Json::arrayValue);
	servers.append(server);
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["servers"] = servers;

	ASSERT(!test.Load(data));
}

TEST_CASE(ApplicationConfig_Load_DuplicateServer)
{
	ApplicationConfig test;
	Json::Value server(Json::objectValue);
	server["host"] = "test-host-name";
	server["client-cert"] = "test-client-cert";
	server["client-key"] = "test-client-key";
	Json::Value servers(Json::arrayValue);
	servers.append(server);
	servers.append(server);
	Json::Value data(Json::objectValue);
	data["cache-dir"] = "test-cache-dir";
	data["cache-valid-seconds"] = 1234321;
	data["servers"] = servers;

	ASSERT(!test.Load(data));
}

