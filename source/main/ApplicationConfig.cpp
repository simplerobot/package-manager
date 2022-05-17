#include "ApplicationConfig.hpp"


ApplicationServerConfig::ApplicationServerConfig()
	: m_has_host(false)
	, m_has_client_cert_file(false)
	, m_has_client_key_file(false)
{
}

bool ApplicationServerConfig::Load(const Json::Value& config)
{
	if (!config.isObject())
	{
		std::printf("Warning: configuration server config does not contain a json object.\n");
		return false;
	}

	bool result = true;

	Json::Value host = config["host"];
	if (host.isString())
	{
		SetHost(host.asString());
	}
	else if (!host.isNull())
	{
		std::printf("Warning: configuration server host must be a string.\n");
		result = false;
	}

	Json::Value client_cert_file = config["client-cert"];
	if (client_cert_file.isString())
	{
		SetClientCertFile(client_cert_file.asString());
	}
	else if (!client_cert_file.isNull())
	{
		std::printf("Warning: configuration client-cert must be a string.\n");
		result = false;
	}

	Json::Value client_key_file = config["client-key"];
	if (client_key_file.isString())
	{
		SetClientKeyFile(client_key_file.asString());
	}
	else if (!client_key_file.isNull())
	{
		std::printf("Warning: configuration client-key must be a string.\n");
		result = false;
	}

	return result;
}

bool ApplicationServerConfig::HasHost() const
{
	return m_has_host;
}

std::string ApplicationServerConfig::GetHost() const
{
	if (!HasHost())
		throw std::runtime_error("missing host");
	return m_host;

}

void ApplicationServerConfig::SetHost(const std::string& host)
{
	m_has_host = true;
	m_host = host;
}

bool ApplicationServerConfig::HasClientCertFile() const
{
	return m_has_client_cert_file;
}

std::string ApplicationServerConfig::GetClientCertFile() const
{
	if (!HasClientCertFile())
		throw std::runtime_error("missing client cert file");
	return m_client_cert_file;
}

void ApplicationServerConfig::SetClientCertFile(const std::string& client_cert_file)
{
	m_has_client_cert_file = true;
	m_client_cert_file = client_cert_file;
}

bool ApplicationServerConfig::HasClientKeyFile() const
{
	return m_has_client_key_file;
}

std::string ApplicationServerConfig::GetClientKeyFile() const
{
	if (!HasClientKeyFile())
		throw std::runtime_error("missing client key file");
	return m_client_key_file;
}

void ApplicationServerConfig::SetClientKeyFile(const std::string& client_key_file)
{
	m_has_client_key_file = true;
	m_client_key_file = client_key_file;
}


ApplicationConfig::ApplicationConfig()
	: m_has_cache_dir(false)
	, m_has_cache_valid_time_s(false)
	, m_cache_valid_time_s(0)
{
}

bool ApplicationConfig::Load(const Json::Value& file)
{
	if (!file.isObject())
	{
		std::printf("Warning: configuration file does not contain a json object.\n");
		return false;
	}

	bool result = true;

	Json::Value cache_dir = file["cache-dir"];
	if (cache_dir.isString())
	{
		SetCacheDir(cache_dir.asString());
	}
	else if (!cache_dir.isNull())
	{
		std::printf("Warning: configuration cache-dir must be a string.\n");
		result = false;
	}

	Json::Value cache_valid_seconds = file["cache-valid-seconds"];
	if (cache_valid_seconds.isUInt())
	{
		SetCacheValidTimeS(cache_valid_seconds.asUInt());
	}
	else if (!cache_valid_seconds.isNull())
	{
		std::printf("Warning: configuration cache-valid-seconds must be a unsigned integer.\n");
		result = false;
	}

	Json::Value server_config_list = file["servers"];
	if (server_config_list.isArray())
	{
		for (auto& server_config : server_config_list)
		{
			ApplicationServerConfig config;
			if (!config.Load(server_config))
			{
				result = false;
			}
			else if (!config.HasHost())
			{
				std::printf("Warning: Server configuration missing host'\n");
				result = false;
			}
			else if (HasServerConfig(config.GetHost()))
			{
				std::printf("Warning: Duplicate server configuration '%s'\n", config.GetHost().c_str());
				result = false;
			}
			else
			{
				AddServerConfig(config);
			}
		}
	}
	else if (!server_config_list.isNull())
	{
		std::printf("Warning: servers configuration must be an array.\n");
		result = false;
	}

	return result;
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

size_t ApplicationConfig::GetServerConfigCount() const
{
	return m_server_configs.size();
}

bool ApplicationConfig::HasServerConfig(const std::string& server_name) const
{
	for (auto& server_config : m_server_configs)
		if (server_config.HasHost() && server_config.GetHost() == server_name)
			return true;
	return false;
}

ApplicationServerConfig ApplicationConfig::GetServerConfig(const std::string& server_name) const
{
	for (auto& server_config : m_server_configs)
		if (server_config.HasHost() && server_config.GetHost() == server_name)
			return server_config;
	throw std::runtime_error("server config not found");
}

void ApplicationConfig::AddServerConfig(const ApplicationServerConfig& config)
{
	if (!config.HasHost())
		throw std::runtime_error("server configuration missing host");
	if (HasServerConfig(config.GetHost()))
		throw std::runtime_error("duplicate server configuration found");
	m_server_configs.push_back(config);
}

