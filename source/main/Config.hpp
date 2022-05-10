#pragma once

#include "Main.hpp"
#include "json/json.h"


class Config
{
public:
	Config();

	bool LoadConfig(const Json::Value& file);

	bool HasCacheDir() const;
	std::string GetCacheDir() const;
	std::string GetCacheDir(const std::string& default_cache_dir) const;
	void SetCacheDir(const std::string& cache_dir);

	bool HasCacheValidTimeS() const;
	size_t GetCacheValidTimeS() const;
	size_t GetCacheValidTimeS(size_t default_cache_valid_time_s) const;
	void SetCacheValidTimeS(size_t cache_valid_time_s);

	bool HasClientCertFile() const;
	std::string GetClientCertFile() const;
	void SetClientCertFile(const std::string& client_cert_file);

	bool HasClientKeyFile() const;
	std::string GetClientKeyFile() const;
	void SetClientKeyFile(const std::string& client_key_file);

private:
	bool m_has_cache_dir;
	std::string m_cache_dir;

	bool m_has_cache_valid_time_s;
	size_t m_cache_valid_time_s;

	bool m_has_client_cert_file;
	std::string m_client_cert_file;

	bool m_has_client_key_file;
	std::string m_client_key_file;
};
