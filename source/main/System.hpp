#pragma once

#include "SystemInterface.hpp"


class System : public SystemInterface
{
public:
	virtual std::string GetConfigurationDir() override;

	virtual bool Load(const std::string& filename, std::string& data_out) override;
};
