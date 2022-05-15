#include "ApplicationConfig.hpp"


ApplicationConfig::ApplicationConfig()
: m_has_cache_dir(false)
, m_has_cache_valid_time_s(false)
, m_cache_valid_time_s(0)
, m_has_client_cert_file(false)
, m_has_client_key_file(false)
{
}

bool ApplicationConfig::Load(const Json::Value& file)
{
	if (!file.isObject())
	{
		std::printf("Warning: configuration file does not contain a json object.\n");
		return false;
	}

	Json::Value cache_dir = file["cache-dir"];
	if (!cache_dir.isNull())
	{
		if (cache_dir.isString())
			SetCacheDir(cache_dir.asString());
		else
			std::printf("Warning: configuration cache-dir must be a string.\n");
	}

	Json::Value cache_valid_seconds = file["cache-valid-seconds"];
	if (!cache_valid_seconds.isNull())
	{
		if (cache_valid_seconds.isUInt())
			SetCacheValidTimeS(cache_valid_seconds.asUInt());
		else
			std::printf("Warning: configuration cache-valid-seconds must be a unsigned integer.\n");
	}

	Json::Value client_cert_file = file["client-cert"];
	if (!client_cert_file.isNull())
	{
		if (client_cert_file.isString())
			SetClientCertFile(client_cert_file.asString());
		else
			std::printf("Warning: configuration client-cert must be a string.\n");
	}

	Json::Value client_key_file = file["client-key"];
	if (!client_key_file.isNull())
	{
		if (client_key_file.isString())
			SetClientKeyFile(client_key_file.asString());
		else
			std::printf("Warning: configuration client-key must be a string.\n");
	}

	return true;
}

bool ApplicationConfig::HasCacheDir() const
{
	return m_has_cache_dir;
}

std::string ApplicationConfig::GetCacheDir() const
{
	if (!HasCacheDir())
		throw std::runtime_error("missing cache dir");
	return m_cache_dir;
}

std::string ApplicationConfig::GetCacheDir(const std::string& default_cache_dir) const
{
	if (!HasCacheDir())
		return default_cache_dir;
	return m_cache_dir;
}

void ApplicationConfig::SetCacheDir(const std::string& cache_dir)
{
	m_has_cache_dir = true;
	m_cache_dir = cache_dir;
}

bool ApplicationConfig::HasCacheValidTimeS() const
{
	return m_has_cache_valid_time_s;
}

size_t ApplicationConfig::GetCacheValidTimeS() const
{
	if (!HasCacheValidTimeS())
		throw std::runtime_error("missing cache valid time s");
	return m_cache_valid_time_s;
}

size_t ApplicationConfig::GetCacheValidTimeS(size_t default_cache_valid_time_s) const
{
	if (!HasCacheValidTimeS())
		return default_cache_valid_time_s;
	return m_cache_valid_time_s;
}

void ApplicationConfig::SetCacheValidTimeS(size_t cache_valid_time_s)
{
	m_has_cache_valid_time_s = true;
	m_cache_valid_time_s = cache_valid_time_s;
}

bool ApplicationConfig::HasClientCertFile() const
{
	return m_has_client_cert_file;
}

std::string ApplicationConfig::GetClientCertFile() const
{
	if (!HasClientCertFile())
		throw std::runtime_error("missing client cert file");
	return m_client_cert_file;
}

void ApplicationConfig::SetClientCertFile(const std::string& client_cert_file)
{
	m_has_client_cert_file = true;
	m_client_cert_file = client_cert_file;
}

bool ApplicationConfig::HasClientKeyFile() const
{
	return m_has_client_key_file;
}

std::string ApplicationConfig::GetClientKeyFile() const
{
	if (!HasClientKeyFile())
		throw std::runtime_error("missing client key file");
	return m_client_key_file;
}

void ApplicationConfig::SetClientKeyFile(const std::string& client_key_file)
{
	m_has_client_key_file = true;
	m_client_key_file = client_key_file;
}

