#pragma once

#include "Main.hpp"


class SystemInterface : public Interface
{
public:
	virtual std::string GetConfigurationDir() = 0;
	virtual bool Load(const std::string& filename, std::string& data_out) = 0;

};
