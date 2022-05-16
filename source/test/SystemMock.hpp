#pragma once

#include "Main.hpp"
#include "SystemInterface.hpp"
#include <map>


class SystemMock : public SystemInterface
{
public:
	SystemMock();
	~SystemMock();

	void SetConfigurationDir(const std::string& config_dir);
	virtual std::string GetConfigurationDir() override;

	void SetFile(const std::string& filename, const std::string& data);
	virtual bool Load(const std::string& filename, std::string& data_out) override;

public:
	size_t m_count_get_configuration_dir;
	bool m_has_configuration_dir;
	std::string m_configuration_dir;

	std::map<std::string, size_t> m_file_counts;
	std::map<std::string, std::string> m_files;
};
