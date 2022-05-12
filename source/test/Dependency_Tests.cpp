#include "Test.hpp"
#include "Dependency.hpp"
#include "json/json.h"


TEST_CASE(Dependency_IsValidName_Valid)
{
	const char* VALID_TEST_CASES[] = {
		"a",
		"a0",
		"a.",
		"_",
		"-",
		"-a",
		"azAZ09_.-",
	};

	for (const char* test_name : VALID_TEST_CASES)
		ASSERT(Dependency::IsValidName(test_name));
}

TEST_CASE(Dependency_IsValidName_Invalid)
{
	const char* INVALID_TEST_CASES[] = {
		"",
		"8",
		".",
		"8a",
		".a",
	};

	for (const char* test_name : INVALID_TEST_CASES)
		ASSERT(!Dependency::IsValidName(test_name));
}

TEST_CASE(Dependency_IsValidName_InitialCharacter)
{
	bool is_valid[256] = {};
	for (int i = 'a'; i <= 'z'; i++)
		is_valid[i] = true;
	for (int i = 'A'; i <= 'Z'; i++)
		is_valid[i] = true;
	is_valid['_'] = true;
	is_valid['-'] = true;

	for (size_t i = 1; i < 256; i++)
	{
		bool expected = is_valid[i];

		std::string a = { (char)i };
		ASSERT(Dependency::IsValidName(a) == expected);

		std::string b = { (char)i, 'x' };
		ASSERT(Dependency::IsValidName(b) == expected);
	}
}

TEST_CASE(Dependency_IsValidName_NextCharacter)
{
	bool is_valid[256] = {};
	for (int i = 'a'; i <= 'z'; i++)
		is_valid[i] = true;
	for (int i = 'A'; i <= 'Z'; i++)
		is_valid[i] = true;
	for (int i = '0'; i <= '9'; i++)
		is_valid[i] = true;
	is_valid['_'] = true;
	is_valid['-'] = true;
	is_valid['.'] = true;

	for (size_t i = 1; i < 256; i++)
	{
		bool expected = is_valid[i];

		std::string a = { 'x', (char)i };
		ASSERT(Dependency::IsValidName(a) == expected);

		std::string b = { 'x', (char)i, 'x' };
		ASSERT(Dependency::IsValidName(b) == expected);
	}
}

TEST_CASE(Dependency_IsValidDir_Valid)
{
	const char* VALID_TEST_CASES[] = {
		"a",
		"a/b",
		".a",
		"a/.b",
		"a/b/c",
	};

	for (const char* test_name : VALID_TEST_CASES)
		ASSERT(Dependency::IsValidDir(test_name));
}

TEST_CASE(Dependency_IsValidDir_Invalid)
{
	const char* INVALID_TEST_CASES[] = {
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

	for (const char* test_name : INVALID_TEST_CASES)
		ASSERT(!Dependency::IsValidDir(test_name));
}

TEST_CASE(Dependency_IsValidDir_Character)
{
	bool is_valid[256] = {};
	for (int i = 'a'; i <= 'z'; i++)
		is_valid[i] = true;
	for (int i = 'A'; i <= 'Z'; i++)
		is_valid[i] = true;
	for (int i = '0'; i <= '9'; i++)
		is_valid[i] = true;
	is_valid['_'] = true;
	is_valid['-'] = true;
	is_valid['.'] = true;

	for (size_t i = 1; i < 256; i++)
	{
		if (i == '/')
			continue;

		bool expected = is_valid[i];

		std::string a = { 'x', (char)i };
		ASSERT(Dependency::IsValidDir(a) == expected);

		std::string b = { 'x', (char)i, 'x' };
		ASSERT(Dependency::IsValidDir(b) == expected);
	}
}

TEST_CASE(Dependency_IsValidUrl_HappyCase)
{
	// TODO: Once we use curl validation, add additional test cases.
	ASSERT(Dependency::IsValidUrl("https://"));
}

TEST_CASE(Dependency_IsValidUrl_Invalid)
{
	// TODO: Once we use curl validation, add additional test cases.
	ASSERT(!Dependency::IsValidUrl("https:/"));
}

TEST_CASE(Dependency_ExtractNameFromUrl_HappyCase)
{
	const char* TESTS[][2] = {
			{ "testfilename", "https://user:password@server:port/path/testfilename.suffix?query#fragment" },
			{ "testfilename", "https://user:password@server:port/path/testfilename.suffix#fragment" },
			{ "testfilename", "https://user:password@server:port/path/testfilename.suffix#fragment?" },
			{ "testfilename", "https://user:password@server:port/path/testfilename.suffix" },
			{ "test.file.name", "https://user:password@server:port/path/test.file.name.suffix" },
			{ "test.file.name", "https://user:password@server:port/path/test.file.name.suffix" },
			{ "testfilename", "https://user:password@server:port/path/testfilename?query#fragment" },
			{ "testfilename", "https://user:password@server:port/path/testfilename" },
			{ "test0filename", "https://user:password@server:port/path/test%30filename" },
			{ "test0", "https://user:password@server:port/path/test%30" },
	};

	for (auto test : TESTS)
	{
		const char* expected = test[0];
		const char* url = test[1];

		std::string actual;
		ASSERT(Dependency::ExtractNameFromUrl(url, actual));
		ASSERT(actual == expected);

	}
}

TEST_CASE(Dependency_ExtractNameFromUrl_Invalid)
{
	const char* TESTS[] = {
		"",
		"http:no slash",
		"https://user:password@server:port/path/?query#fragment",
		"https://user:password@server:port/path/%.suffix?query#fragment",
		"https://user:password@server:port/path/%f.suffix?query#fragment",
		"https://user:password@server:port/path/%fg.suffix?query#fragment",
		"https://user:password@server:port/path/%",
	};

	for (auto test : TESTS)
	{
		std::string actual;
		ASSERT(!Dependency::ExtractNameFromUrl(test, actual));
	}
}

TEST_CASE(Dependency_Constructor)
{
	Dependency test;

	ASSERT(test.GetUrl() == "");
	ASSERT(test.GetName() == "");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "");
}

TEST_CASE(Dependency_SetUrl)
{
	Dependency test;

	test.SetUrl("https://user:password@server:port/path/testname.suffix?query#fragment");

	ASSERT(test.GetUrl() == "https://user:password@server:port/path/testname.suffix?query#fragment");
	ASSERT(test.GetName() == "testname");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "a/testname");
}

TEST_CASE(Dependency_SetName)
{
	Dependency test;

	test.SetName("testname");

	ASSERT(test.GetUrl() == "");
	ASSERT(test.GetName() == "testname");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "a/testname");
}

TEST_CASE(Dependency_SetDir)
{
	Dependency test;

	test.SetDir("testdir");

	ASSERT(test.GetUrl() == "");
	ASSERT(test.GetName() == "");
	ASSERT(test.GetDir() == "testdir");
	ASSERT(test.GetDir("a") == "testdir");
}

TEST_CASE(Dependency_Set_JsonString)
{
	Dependency test;
	Json::Value value = "testurl";

	ASSERT(test.Set(value));

	ASSERT(test.GetUrl() == "testurl");
	ASSERT(test.GetName() == "");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "");
}

TEST_CASE(Dependency_Set_JsonObject_HappyCase)
{
	Dependency test;
	Json::Value value(Json::objectValue);
	value["url"] = "test_url";
	value["name"] = "test_name";
	value["dir"] = "test_dir";

	ASSERT(test.Set(value));

	ASSERT(test.GetUrl() == "test_url");
	ASSERT(test.GetName() == "test_name");
	ASSERT(test.GetDir() == "test_dir");
	ASSERT(test.GetDir("a") == "test_dir");
}

TEST_CASE(Dependency_Set_JsonObject_InvalidUrl)
{
	Dependency test;
	Json::Value value(Json::objectValue);
	value["url"] = 5;
	value["name"] = "test_name";
	value["dir"] = "test_dir";

	ASSERT(!test.Set(value));

	ASSERT(test.GetUrl() == "");
	ASSERT(test.GetName() == "test_name");
	ASSERT(test.GetDir() == "test_dir");
	ASSERT(test.GetDir("a") == "test_dir");
}

TEST_CASE(Dependency_Set_JsonObject_InvalidName)
{
	Dependency test;
	Json::Value value(Json::objectValue);
	value["url"] = "test_url";
	value["name"] = 5;
	value["dir"] = "test_dir";

	ASSERT(!test.Set(value));

	ASSERT(test.GetUrl() == "test_url");
	ASSERT(test.GetName() == "");
	ASSERT(test.GetDir() == "test_dir");
	ASSERT(test.GetDir("a") == "test_dir");
}

TEST_CASE(Dependency_Set_JsonObject_InvalidDir)
{
	Dependency test;
	Json::Value value(Json::objectValue);
	value["url"] = "test_url";
	value["name"] = "test_name";
	value["dir"] = 5;

	ASSERT(!test.Set(value));

	ASSERT(test.GetUrl() == "test_url");
	ASSERT(test.GetName() == "test_name");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "a/test_name");
}

TEST_CASE(Dependency_Set_JsonNull)
{
	Dependency test;
	Json::Value value;

	ASSERT(!test.Set(value));

	ASSERT(test.GetUrl() == "");
	ASSERT(test.GetName() == "");
	ASSERT(test.GetDir() == "");
	ASSERT(test.GetDir("a") == "");
}

TEST_CASE(Dependency_IsValid_A_HappyCase)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/test_dir");

	ASSERT(test.IsValid());
}

TEST_CASE(Dependency_IsValid_A_InvalidUrl)
{
	Dependency test;
	test.SetUrl("http://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/test_dir");

	ASSERT(!test.IsValid());
}

TEST_CASE(Dependency_IsValid_A_InvalidName)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("*");
	test.SetDir("a/test_dir");

	ASSERT(!test.IsValid());
}

TEST_CASE(Dependency_IsValid_A_InvalidDir)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/../test_dir");

	ASSERT(!test.IsValid());
}

TEST_CASE(Dependency_IsValid_A_MissingName)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetDir("a/test_dir");

	ASSERT(test.IsValid());
}

TEST_CASE(Dependency_IsValid_A_MissingDir)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");

	ASSERT(!test.IsValid());
}

TEST_CASE(Dependency_IsValid_B_HappyCase)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/test_dir");

	ASSERT(test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_InvalidUrl)
{
	Dependency test;
	test.SetUrl("http://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/test_dir");

	ASSERT(!test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_InvalidName)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("*");
	test.SetDir("a/test_dir");

	ASSERT(!test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_InvalidDir)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");
	test.SetDir("a/../test_dir");

	ASSERT(!test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_MissingName)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetDir("a/test_dir");

	ASSERT(test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_MissingDir)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");
	test.SetName("test_name");

	ASSERT(test.IsValid("b"));
}

TEST_CASE(Dependency_IsValid_B_MissingDirAndName)
{
	Dependency test;
	test.SetUrl("https://user:password@server:port/path/test_name.suffix?query#fragment");

	ASSERT(test.IsValid("b"));
}
