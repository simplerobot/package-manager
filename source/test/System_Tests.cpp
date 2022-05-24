#include "Test.hpp"
#include "System.hpp"
#include <unistd.h>
#include <cstring>


TEST_CASE(System_Constructor)
{
	System test;
}

TEST_CASE(System_GetConfigurationDir_HappyCase)
{
	System test;

	std::string config_dir = test.GetConfigurationDir();

	ASSERT(!config_dir.empty());
	ASSERT(config_dir.compare(0, 6, "/home/") == 0);
	ASSERT(config_dir.back() != '/');
}

TEST_CASE(System_GetConfigurationDir_Writeable)
{
	System test;
	std::string config_dir = test.GetConfigurationDir() + "/";

    int result = ::access(config_dir.c_str(), W_OK);

    ASSERT(result == 0);
}

TEST_CASE(System_Load_HappyCase)
{
	System test;
	std::string data;

	ASSERT(test.Load("build/test/small.txt", data));

	ASSERT(data == "abcd1234");
}

TEST_CASE(System_Load_LargeFile)
{
	System test;
	std::string data;
	const char* SOURCE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	size_t LENGTH = std::strlen(SOURCE);

	ASSERT(test.Load("build/test/large.txt", data));

	ASSERT(data.size() == LENGTH * LENGTH + LENGTH);
	auto cursor = data.begin();
	for (size_t i = 0; SOURCE[i] != 0; i++)
	{
		ASSERT(*(cursor++) == SOURCE[i]);
		for (size_t j = 0; SOURCE[j] != 0; j++)
			ASSERT(*(cursor++) == SOURCE[j]);
	}
}

TEST_CASE(System_Load_MissingFile)
{
	System test;
	std::string data;

	ASSERT(!test.Load("build/test/missing.txt", data));
}


