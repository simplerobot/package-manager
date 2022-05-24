#include "System.hpp"
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <fstream>
#include <sstream>


std::string System::GetConfigurationDir()
{
	const char* config_dir = nullptr;
	if (config_dir == nullptr)
		config_dir = ::getenv("XDG_CONFIG_HOME");
	if (config_dir == nullptr)
		config_dir = ::getenv("HOME");
	if (config_dir == nullptr)
		config_dir = ::getpwuid(::getuid())->pw_dir;
	if (config_dir == nullptr)
		throw std::runtime_error("no configuration directory found");
	return config_dir;
}

bool System::Load(const std::string& filename, std::string& data_out)
{
	// Open the file.
	std::ifstream input(filename, std::ios::binary);
	if (!input.is_open() || input.bad())
		return false;

	// Read blocks of data into the buffer.  This is double copying, so it is not terribly efficient.
	char buffer[1024];
	std::ostringstream out;
	while (input.read(buffer, sizeof(buffer)))
		out.write(buffer, sizeof(buffer));
	out.write(buffer, input.gcount());

	data_out = out.str();
	return true;
}

