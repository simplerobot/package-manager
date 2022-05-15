#pragma once

#include "Main.hpp"
#include "json/json.h"


class PackageConfig
{
public:
	typedef std::vector<PackageConfig>::iterator iterator;
	typedef std::vector<PackageConfig>::const_iterator const_iterator;

public:
	PackageConfig();

	bool Load(const Json::Value& value);

	bool HasName() const;
	bool ValidateName() const;
	std::string GetName() const;
	void SetName(const std::string& name);

	bool HasConfigUrl() const;
	bool ValidateConfigUrl() const;
	std::string GetConfigUrl() const;
	void SetConfigUrl(const std::string& url);

	bool HasBinaryUrl() const;
	bool ValidateBinaryUrl() const;
	std::string GetBinaryUrl() const;
	void SetBinaryUrl(const std::string& url);

	bool HasReleaseDir() const;
	bool ValidateReleaseDir() const;
	std::string GetReleaseDir() const;
	void SetReleaseDir(const std::string& dir);

	bool HasImportDir() const;
	bool ValidateImportDir() const;
	std::string GetImportDir() const;
	void SetImportDir(const std::string& dir);

	bool ValidateDependencies() const;
	void AddDependency(const PackageConfig& config);

	// Iterate through dependencies.
	iterator begin();
	iterator end();
	const_iterator begin() const;
	const_iterator end() const;

	// Validation methods exposed to simplify testing.
	static bool ValidateUrl(const std::string& url, const char* field);
	static bool ValidateToken(const std::string& token, const char* field);
	static bool ValidateDir(const std::string& dir, const char* field);

private:
	std::string m_name;
	std::string m_config_url;
	std::string m_binary_url;
	std::string m_release_dir;
	std::string m_import_dir;
	std::vector<PackageConfig> m_dependencies;
};
