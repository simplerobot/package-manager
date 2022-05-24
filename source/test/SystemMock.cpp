#include "SystemMock.hpp"
#include "Test.hpp"


SystemMock::SystemMock()
	: m_count_get_configuration_dir(0)
	, m_has_configuration_dir(false)
{
}

SystemMock::~SystemMock()
{
	ASSERT(!m_has_configuration_dir || m_count_get_configuration_dir > 0);
	for (auto& i : m_files)
		ASSERT(m_file_counts[i.first] > 0);
}

void SystemMock::SetConfigurationDir(const std::string& config_dir)
{
	m_has_configuration_dir = true;
	m_configuration_dir = config_dir;
}

std::string SystemMock::GetConfigurationDir()
{
	m_count_get_configuration_dir++;
	ASSERT(m_has_configuration_dir);
	return m_configuration_dir;
}

void SystemMock::SetFile(const std::string& filename, const std::string& data)
{
	ASSERT(m_files.find(filename) == m_files.end());
	m_files[filename] = data;
}

bool SystemMock::Load(const std::string& filename, std::string& data_out)
{
	m_file_counts[filename]++;
	auto i = m_files.find(filename);
	if (i == m_files.end())
		return false;
	data_out = i->second;
	return true;
}

