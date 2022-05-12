#include "Dependency.hpp"


Dependency::Dependency()
{
}

bool Dependency::Set(const Json::Value& value)
{
	Json::Value url;
	Json::Value name;
	Json::Value dir;

	if (value.isString())
	{
		url = value;
	}
	else if (value.isObject())
	{
		url = value["url"];
		name = value["name"];
		dir = value["dir"];
	}
	else
	{
		std::printf("Warning: Dependency must be a url string or an object with a url string.\n");
		return false;
	}

	bool result = true;

	if (url.isNull())
	{
		std::printf("Warning: Dependency must include a url.\n");
		result = false;
	}
	else if (!url.isString())
	{
		std::printf("Warning: Dependency url must be a string.\n");
		result = false;
	}
	else
	{
		SetUrl(url.asString());
	}

	if (!name.isNull())
	{
		if (!name.isString())
		{
			std::printf("Warning: Dependency name must be a string.\n");
			result = false;
		}
		else
		{
			SetName(name.asString());
		}
	}

	if (!dir.isNull())
	{
		if (!dir.isString())
		{
			std::printf("Warning: Dependency dir must be a string.\n");
			result = false;
		}
		else
		{
			SetDir(dir.asString());
		}
	}

	return result;
}

void Dependency::SetName(const std::string& name)
{
	m_name = name;
}

void Dependency::SetUrl(const std::string& url)
{
	m_url = url;
}

void Dependency::SetDir(const std::string& dir)
{
	m_dir = dir;
}

bool Dependency::IsValid() const
{
	return IsValidUrl(GetUrl()) && IsValidName(GetName()) && IsValidDir(GetDir());
}

bool Dependency::IsValid(const std::string& default_dir) const
{
	return IsValidUrl(GetUrl()) && IsValidName(GetName()) && IsValidDir(GetDir(default_dir));
}

std::string Dependency::GetUrl() const
{
	return m_url;
}

std::string Dependency::GetName() const
{
	if (!m_name.empty())
		return m_name;
	std::string name;
	if (ExtractNameFromUrl(m_url, name))
		return name;
	return std::string();
}

std::string Dependency::GetDir() const
{
	return m_dir;
}

std::string Dependency::GetDir(const std::string& default_dir) const
{
	if (!m_dir.empty())
		return m_dir;
	std::string name = GetName();
	if (!default_dir.empty() && !name.empty())
		return default_dir + "/" + GetName();
	return std::string();
}

static bool IsValidNameInitialChar(char c)
{
	return std::isalpha(c) || c == '_' || c == '-';
}

static bool IsValidNameNextChar(char c)
{
	return IsValidNameInitialChar(c) || std::isdigit(c) || c == '.';
}

static bool IsValidDirSectionChar(char c)
{
	return std::isalnum(c) || c == '_' || c == '-' || c == '.';
}

static bool IsValidName(const char* s)
{
	// We are very strict on allowed project names.  Consider security implications before relaxing this.
	if (!IsValidNameInitialChar(*(s++)))
		return false;
	while (*s != 0)
		if (!IsValidNameNextChar(*(s++)))
			return false;
	return true;
}

static bool IsValidDir(const char* s)
{
	// We are very strict on allowed directory names.  Consider security implications before relaxing this.
	bool is_initial = true;
	bool is_initial_dot = false;
	while (true)
	{
		char c = *(s++);
		if (c == '/' || c == 0)
		{
			if (is_initial)
				return false; // Don't allow absolute directories or multiple '/' characters in a row.
			if (is_initial_dot)
				return false; // Don't allow any "/./" type elements.
			if (c == 0)
				return true; // Successfully reached the end of the dir string.
			is_initial = true;
			is_initial_dot = false;
		}
		else if (c == '.')
		{
			if (is_initial_dot)
				return false; // Make sure we can't escape a directory.  This disallows any path element starting with "..", which is a bit overly strict.
			if (is_initial)
				is_initial_dot = true;
			is_initial = false;
		}
		else if (IsValidDirSectionChar(c))
		{
			is_initial = false;
			is_initial_dot = false;
		}
		else
			return false; // Invalid character in path.
	}
}

bool Dependency::IsValidUrl(const std::string& url)
{
	if (url.compare(0, 8, "https://") != 0)
		return false;
	// TODO: use curl_url_set to validate the basic structure of the url
	return true;
}

bool Dependency::IsValidName(const std::string& name)
{
	return ::IsValidName(name.data());
}

bool Dependency::IsValidDir(const std::string& dir)
{
	return ::IsValidDir(dir.data());
}

bool Dependency::ExtractNameFromUrl(const std::string& url, std::string& name_out)
{
	// TODO: use curl to extract the path.

	// Ignore any query or fragment commands in the url.
	size_t end = 0;
	while (end < url.size() && url[end] != '?' && url[end] != '#')
		end++;

	// Start after the last '/' in the path.
	size_t start = 0;
	for (size_t i = 0; i < end; i++)
		if (url[i] == '/')
			start = i + 1;
	if (start == 0)
		return false;

	// Ignore the final suffix of the file.
	size_t new_end = end;
	for (size_t i = start; i < end; i++)
		if (url[i] == '.')
			new_end = i;

	if (start == new_end)
		return false;

	// Decode percent encoded strings.
	std::ostringstream out;
	size_t cursor = start;
	while (cursor < new_end)
	{
		char c = url[cursor++];
		if (c == '%')
		{
			if (cursor + 2 > new_end)
				return false;
			char x[3] = {};
			for (size_t i = 0; i < 2; i++)
			{
				x[i] = url[cursor++];
				if (!::isxdigit(x[i]))
					return false;
			}
			out.put((char)strtol(x, nullptr, 16));
		}
		else
			out.put(c);
	}

	name_out = out.str();
	return true;
}
