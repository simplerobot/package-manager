#include "Test.hpp"
#include "PackageConfig.hpp"


TEST_CASE(PackageConfig_ValidateUrl_ValidUrls)
{
	const char* TESTS[] = {
		"https://",
		"https://user:password@server:port/path/testfilename.suffix?query#fragment",
	};

	for (const char* test : TESTS)
		ASSERT(PackageConfig::ValidateUrl(test, "none"));
}

TEST_CASE(PackageConfig_ValidateUrl_InvalidUrls)
{
	const char* TESTS[] = {
		"",
		"http://",
		"http://user:password@server:port/path/testfilename.suffix?query#fragment",
		"file://local",
	};
	// TODO: Add additional validations once we add curl structure validation.

	for (const char* test : TESTS)
		ASSERT(!PackageConfig::ValidateUrl(test, "none"));
}

TEST_CASE(PackageConfig_ValidateUrl_Characters)
{
	const char* VALID_URL_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~:/?#[]@!$&'()*+,;=%";
	bool is_valid[256] = {};
	for (size_t i = 0; VALID_URL_CHARACTERS[i] != 0; i++)
		is_valid[(uint8_t)VALID_URL_CHARACTERS[i]] = true;

	for (size_t i = 0; i < 256; i++)
	{
		bool expected = is_valid[i];
		std::string url = "https://";
		url += (char)i;

		ASSERT(PackageConfig::ValidateUrl(url, "none") == expected);
	}
}

TEST_CASE(PackageConfig_ValidateToken_Valid)
{
	const char* TESTS[] = {
		"a",
		"a0",
		"a.",
		"_",
		"-",
		"-a",
		"azAZ09_.-",
	};

	for (const char* test : TESTS)
		ASSERT(PackageConfig::ValidateToken(test, "none"));
}

TEST_CASE(PackageConfig_ValidateToken_Invalid)
{
	const char* TESTS[] = {
		"",
		"8",
		".",
		"8a",
		".a",
	};

	for (const char* test : TESTS)
		ASSERT(!PackageConfig::ValidateToken(test, "none"));
}

TEST_CASE(PackageConfig_ValidateToken_InitialCharacter)
{
	const char* VALID_TOKEN_INITIAL_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_-";
	bool is_valid[256] = {};
	for (size_t i = 0; VALID_TOKEN_INITIAL_CHARACTERS[i] != 0; i++)
		is_valid[(uint8_t)VALID_TOKEN_INITIAL_CHARACTERS[i]] = true;

	for (size_t i = 0; i < 256; i++)
	{
		bool expected = is_valid[i];

		std::string a = { (char)i };
		ASSERT(PackageConfig::ValidateToken(a, "none") == expected);

		std::string b = { (char)i, 'x' };
		ASSERT(PackageConfig::ValidateToken(b, "none") == expected);
	}
}

TEST_CASE(PackageConfig_ValidateToken_NextCharacter)
{
	const char* VALID_TOKEN_NEXT_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.";
	bool is_valid[256] = {};
	for (size_t i = 0; VALID_TOKEN_NEXT_CHARACTERS[i] != 0; i++)
		is_valid[(uint8_t)VALID_TOKEN_NEXT_CHARACTERS[i]] = true;

	for (size_t i = 0; i < 256; i++)
	{
		bool expected = is_valid[i];

		std::string a = { 'x', (char)i };
		ASSERT(PackageConfig::ValidateToken(a, "none") == expected);

		std::string b = { 'x', (char)i, 'x' };
		ASSERT(PackageConfig::ValidateToken(b, "none") == expected);
	}
}

TEST_CASE(PackageConfig_ValidateDir_Valid)
{
	const char* TESTS[] = {
		"a",
		"a/b",
		".a",
		"a/.b",
		"a/b/c",
	};

	for (const char* test : TESTS)
		ASSERT(PackageConfig::ValidateDir(test, "none"));
}

TEST_CASE(PackageConfig_ValidateDir_Invalid)
{
	const char* TESTS[] = {
		"",
		"/a",
		"a/",
		"a//b",
		"a/./b",
		"..",
		".",
		"a/..",
		"a/../b",
		"a/..b",
		" ",
		"a/ ..",
	};

	for (const char* test : TESTS)
		ASSERT(!PackageConfig::ValidateDir(test, "none"));
}

TEST_CASE(PackageConfig_ValidateDir_Character)
{
	const char* VALID_DIR_CHARACTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_-.";
	bool is_valid[256] = {};
	for (size_t i = 0; VALID_DIR_CHARACTERS[i] != 0; i++)
		is_valid[(uint8_t)VALID_DIR_CHARACTERS[i]] = true;

	for (size_t i = 0; i < 256; i++)
	{
		if (i == '/')
			continue;

		bool expected = is_valid[i];

		std::string a = { 'x', (char)i };
		ASSERT(PackageConfig::ValidateDir(a, "none") == expected);

		std::string b = { 'x', (char)i, 'x' };
		ASSERT(PackageConfig::ValidateDir(b, "none") == expected);
	}
}

TEST_CASE(PackageConfig_Constructor)
{
	PackageConfig test;
}

TEST_CASE(PackageConfig_SetName)
{
	PackageConfig test;

	ASSERT(!test.HasName());
	ASSERT_THROWS(test.GetName());

	test.SetName("test");

	ASSERT(test.HasName());
	ASSERT(test.GetName() == "test");
}

TEST_CASE(PackageConfig_SetConfigUrl)
{
	PackageConfig test;

	ASSERT(!test.HasConfigUrl());
	ASSERT_THROWS(test.GetConfigUrl());

	test.SetConfigUrl("test");

	ASSERT(test.HasConfigUrl());
	ASSERT(test.GetConfigUrl() == "test");
}

TEST_CASE(PackageConfig_SetBinaryUrl)
{
	PackageConfig test;

	ASSERT(!test.HasBinaryUrl());
	ASSERT_THROWS(test.GetBinaryUrl());

	test.SetBinaryUrl("test");

	ASSERT(test.HasBinaryUrl());
	ASSERT(test.GetBinaryUrl() == "test");
}

TEST_CASE(PackageConfig_SetReleaseDir)
{
	PackageConfig test;

	ASSERT(!test.HasReleaseDir());
	ASSERT_THROWS(test.GetReleaseDir());

	test.SetReleaseDir("test");

	ASSERT(test.HasReleaseDir());
	ASSERT(test.GetReleaseDir() == "test");
}

TEST_CASE(PackageConfig_SetImportDir)
{
	PackageConfig test;

	ASSERT(!test.HasImportDir());
	ASSERT_THROWS(test.GetImportDir());

	test.SetImportDir("test");

	ASSERT(test.HasImportDir());
	ASSERT(test.GetImportDir() == "test");
}

TEST_CASE(PackageConfig_SetAll)
{
	PackageConfig test;

	test.SetName("name");
	test.SetConfigUrl("config-url");
	test.SetBinaryUrl("binary-url");
	test.SetReleaseDir("release-dir");
	test.SetImportDir("import-dir");

	ASSERT(test.GetName() == "name");
	ASSERT(test.GetConfigUrl() == "config-url");
	ASSERT(test.GetBinaryUrl() == "binary-url");
	ASSERT(test.GetReleaseDir() == "release-dir");
	ASSERT(test.GetImportDir() == "import-dir");
}

TEST_CASE(PackageConfig_AddDependency)
{
	PackageConfig test;
	PackageConfig a;
	a.SetName("name-a");
	PackageConfig b;
	b.SetName("name-b");

	ASSERT(test.begin() == test.end());

	test.AddDependency(a);
	test.AddDependency(b);

	auto i = test.begin();
	ASSERT(i != test.end());
	ASSERT(i->GetName() == "name-a");
	i++;
	ASSERT(i != test.end());
	ASSERT(i->GetName() == "name-b");
	i++;
	ASSERT(i == test.end());
}

TEST_CASE(PackageConfig_Set_HappyCase)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(test.Set(config));

	ASSERT(test.GetName() == "test-name");
	ASSERT(test.GetConfigUrl() == "test-config-url");
	ASSERT(test.GetBinaryUrl() == "test-binary-url");
	ASSERT(test.GetReleaseDir() == "test-release-dir");
	ASSERT(test.GetImportDir() == "test-import-dir");
	auto i = test.begin();
	ASSERT(i != test.end());
	ASSERT(i->GetName() == "dependency-name");
	i++;
	ASSERT(i == test.end());
}

TEST_CASE(PackageConfig_Set_InvalidName)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = 5;
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_Invalid)
{
	PackageConfig test;
	Json::Value config(Json::stringValue);

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidConfigUrl)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = 5;
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidBinaryUrl)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = 5;
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidReleaseDir)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = 5;
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidImportDir)
{
	PackageConfig test;
	Json::Value dependency(Json::objectValue);
	dependency["name"] = "dependency-name";
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = dependency;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = 5;
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidDepenency)
{
	PackageConfig test;
	Json::Value dependency_array(Json::arrayValue);
	dependency_array[0] = 5;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = dependency_array;

	ASSERT(!test.Set(config));
}

TEST_CASE(PackageConfig_Set_InvalidDependencyArray)
{
	PackageConfig test;
	Json::Value config(Json::objectValue);
	config["name"] = "test-name";
	config["config-url"] = "test-config-url";
	config["binary-url"] = "test-binary-url";
	config["release-dir"] = "test-release-dir";
	config["import-dir"] = "test-import-dir";
	config["dependencies"] = 5;

	ASSERT(!test.Set(config));
}
