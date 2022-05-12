#pragma once

#include "Main.hpp"
#include "json/json.h"


class Dependency
{
public:
	Dependency();

	bool Set(const Json::Value& value);

	void SetUrl(const std::string& url);
	void SetName(const std::string& name);
	void SetDir(const std::string& dir);

	bool IsValid() const;
	bool IsValid(const std::string& default_dir) const;

	std::string GetUrl() const;
	std::string GetName() const;
	std::string GetDir() const;
	std::string GetDir(const std::string& default_dir) const;

	static bool IsValidUrl(const std::string& url);
	static bool IsValidName(const std::string& name);
	static bool IsValidDir(const std::string& dir);

	static bool ExtractNameFromUrl(const std::string& url, std::string& name_out);

private:
	std::string m_name;
	std::string m_url;
	std::string m_dir;
};
