#include "PackageConfig.hpp"


PackageConfig::PackageConfig()
{
}

static bool SetOptionalString(const Json::Value& value, std::string& output, const char* field)
{
	bool result = true;

	if (value.isString())
	{
		output = value.asString();
	}
	else if (!value.isNull())
	{
		std::printf("Error: Field %s contains a non-string value.\n", field);
		result = false;
	}

	return result;
}

bool PackageConfig::Load(const Json::Value& value)
{
	if (!value.isObject())
	{
		std::printf("Error: Package Configuration must be a JSON object.\n");
		return false;
	}

	bool result = true;
	if (!SetOptionalString(value["name"], m_name, "name"))
		result = false;
	if (!SetOptionalString(value["config-url"], m_config_url, "config-url"))
		result = false;
	if (!SetOptionalString(value["binary-url"], m_binary_url, "binary-url"))
		result = false;
	if (!SetOptionalString(value["release-dir"], m_release_dir, "release-dir"))
		result = false;
	if (!SetOptionalString(value["import-dir"], m_import_dir, "import-dir"))
		result = false;

	Json::Value dependencies = value["dependencies"];

	if (dependencies.isArray())
	{
		for (auto& dependency : dependencies)
		{
			PackageConfig package;
			if (!package.Load(dependency))
				result = false;
			AddDependency(package);
		}
	}
	else if (!dependencies.isNull())
	{
		std::printf("Error: Field dependencies contains a non-array value.\n");
		result = false;
	}

	return result;
}

bool PackageConfig::HasName() const
{
	return !m_name.empty();
}

bool PackageConfig::ValidateName() const
{
	return ValidateToken(m_name, "name");
}

std::string PackageConfig::GetName() const
{
	if (!HasName())
		throw std::runtime_error("missing name");
	return m_name;
}

void PackageConfig::SetName(const std::string& name)
{
	m_name = name;
}

bool PackageConfig::HasConfigUrl() const
{
	return !m_config_url.empty();
}

bool PackageConfig::ValidateConfigUrl() const
{
	return ValidateUrl(m_config_url, "config-url");
}

std::string PackageConfig::GetConfigUrl() const
{
	if (!HasConfigUrl())
		throw std::runtime_error("missing config-url");
	return m_config_url;
}

void PackageConfig::SetConfigUrl(const std::string& url)
{
	m_config_url = url;
}

bool PackageConfig::HasBinaryUrl() const
{
	return !m_binary_url.empty();
}

bool PackageConfig::ValidateBinaryUrl() const
{
	return ValidateUrl(m_binary_url, "binary-url");
}

std::string PackageConfig::GetBinaryUrl() const
{
	if (!HasBinaryUrl())
		throw std::runtime_error("missing config-url");
	return m_binary_url;
}

void PackageConfig::SetBinaryUrl(const std::string& url)
{
	m_binary_url = url;
}

bool PackageConfig::HasReleaseDir() const
{
	return !m_release_dir.empty();
}

bool PackageConfig::ValidateReleaseDir() const
{
	return ValidateDir(m_release_dir, "release-dir");
}

std::string PackageConfig::GetReleaseDir() const
{
	if (!HasReleaseDir())
		throw std::runtime_error("missing release-dir");
	return m_release_dir;
}

void PackageConfig::SetReleaseDir(const std::string& dir)
{
	m_release_dir = dir;
}

bool PackageConfig::HasImportDir() const
{
	return !m_import_dir.empty();
}

bool PackageConfig::ValidateImportDir() const
{
	return ValidateDir(m_import_dir, "import-dir");
}

std::string PackageConfig::GetImportDir() const
{
	if (!HasImportDir())
		throw std::runtime_error("missing import-dir");
	return m_import_dir;
}

void PackageConfig::SetImportDir(const std::string& dir)
{
	m_import_dir = dir;
}

bool PackageConfig::ValidateDependencies() const
{
	throw std::runtime_error("not implemented");
}

void PackageConfig::AddDependency(const PackageConfig& config)
{
	m_dependencies.push_back(config);
}

PackageConfig::iterator PackageConfig::begin()
{
	return m_dependencies.begin();
}

PackageConfig::iterator PackageConfig::end()
{
	return m_dependencies.end();
}

PackageConfig::const_iterator PackageConfig::begin() const
{
	return m_dependencies.begin();
}

PackageConfig::const_iterator PackageConfig::end() const
{
	return m_dependencies.end();
}

static bool IsValidUrlChar(char c)
{
	return std::isalnum(c) || (c != 0 && std::strchr("-._~:/?#[]@!$&'()*+,;=%", c) != nullptr);
}

static bool IsValidTokenInitialChar(char c)
{
	return std::isalpha(c) || c == '_' || c == '-';
}

static bool IsValidTokenNextChar(char c)
{
	return IsValidTokenInitialChar(c) || std::isdigit(c) || c == '.';
}

static bool IsValidDirSectionChar(char c)
{
	return std::isalnum(c) || c == '_' || c == '-' || c == '.';
}

static bool ValidateUrl(const char* begin, const char* end, const char* field)
{
	if (begin == end)
	{
		std::printf("Error: Url %s is empty.\n", field);
		return false;
		// empty.
	}
	for (const char* s = begin; s != end; s++)
	{
		if (!IsValidUrlChar(*s))
		{
			std::printf("Error: Url %s contains invalid character '%c' 0x%02x.\n", field, std::isprint(*s) ? *s : '?', (uint8_t)*s);
			return false;
		}
	}
	if (std::strncmp(begin, "https://", 8) != 0)
	{
		std::printf("Error: Url %s must be a https url.\n", field);
		return false;
	}
	// TODO: use curl_url_set to validate the basic structure of the url
	return true;
}

static bool ValidateToken(const char* begin, const char* end, const char* field)
{
	// We are very strict on allowed project names.  Consider security implications before relaxing this.
	if (begin == end)
	{
		std::printf("Error: token %s empty.\n", field);
		return false;
	}
	for (const char* s = begin; s != end; s++)
	{
		if ((s == begin) ? !IsValidTokenInitialChar(*s) : !IsValidTokenNextChar(*s))
		{
			std::printf("Error: Token %s contains invalid character '%c' 0x%02x.\n", field, std::isprint(*s) ? *s : '?', (uint8_t)*s);
			return false;
		}
	}
	return true;
}

static bool ValidateDir(const char* begin, const char* end, const char* field)
{
	// We are very strict on allowed directory names.  Consider security implications before relaxing this.
	bool is_start = true;
	bool is_initial = true;
	bool is_initial_dot = false;
	for (const char* s = begin; s != end; s++)
	{
		if (*s == '/')
		{
			if (is_start)
			{
				std::printf("Error: Directory path %s may not be absolute.\n", field);
				return false;
			}
			if (is_initial)
			{
				std::printf("Error: Directory path %s includes two slash characters in a row.\n", field);
				return false;
			}
			if (is_initial_dot)
			{
				std::printf("Error: Directory path %s includes a dot element.\n", field);
				return false;
			}
			is_initial = true;
			is_initial_dot = false;
		}
		else if (*s == '.')
		{
			if (is_initial_dot)
			{
				std::printf("Error: Directory path %s may not include a parent element.\n", field);
				return false;
			}
			if (is_initial)
				is_initial_dot = true;
			is_start = false;
			is_initial = false;
		}
		else if (IsValidDirSectionChar(*s))
		{
			is_start = false;
			is_initial = false;
			is_initial_dot = false;
		}
		else
		{
			std::printf("Error: Directory path %s contains invalid character '%c' 0x%02x.\n", field, std::isprint(*s) ? *s : '?', (uint8_t)*s);
			return false;
		}
	}
	if (is_start)
	{
		std::printf("Error: Directory path %s is empty.\n", field);
		return false;
	}
	if (is_initial)
	{
		std::printf("Error: Directory path %s ends with a slash.\n", field);
		return false;
	}
	if (is_initial_dot)
	{
		std::printf("Error: Directory path %s ends with a dot element.\n", field);
		return false;
	}
	return true;
}

bool PackageConfig::ValidateUrl(const std::string& url, const char* field)
{
	return ::ValidateUrl(url.data(), url.data() + url.size(), field);
}

bool PackageConfig::ValidateToken(const std::string& token, const char* field)
{
	return ::ValidateToken(token.data(), token.data() + token.size(), field);
}

bool PackageConfig::ValidateDir(const std::string& dir, const char* field)
{
	return ::ValidateDir(dir.data(), dir.data() + dir.size(), field);
}

