#include "Common.hpp"

#include <cctype>

namespace FishEngine
{
	std::string CamelCaseToReadable(const std::string& str)
	{
		std::string ret;
		ret.reserve(str.size() * 2); // at most: ceil(str.size()/2)*3
		ret.push_back(str[0]);
		for (size_t i = 1; i < str.size(); ++i) {
			char c = str[i];
			if (isupper(c) && !isupper(str[i - 1]))
				ret.push_back(' ');
			ret.push_back(c);
		}
		//ret.push_back(0);
		return ret;
	}
	
	std::string Capitalize(const std::string& str)
	{
		std::string ret = str;
		if (std::islower(ret[0]))
		{
			ret[0] = std::toupper(ret[0]);
		}
		return ret;
	}

#if 0
	std::vector<std::string> split(const std::string& str, const std::string& separator)
	{
		auto l = separator.length();
		auto pos = str.find_first_of(separator);
		decltype(pos) last_pos = 0;
		std::vector<std::string> ret;
		while (pos != std::string::npos) {
			ret.push_back(str.substr(last_pos, pos - last_pos));
			pos += l;
			last_pos = pos;
			pos = str.find_first_of(separator, pos);
		}
		ret.push_back(str.substr(last_pos, pos));
		return ret;
	}

	std::string trim(const std::string& str) {
		//str.erase(0, str.find_first_not_of(' '));
		//str.erase(str.find_last_not_of(' ')+1);
		auto begin = str.find_first_not_of(" \t\r\n");
		auto end = str.find_last_not_of(" \t\t\n");
		return str.substr(begin, end);
	}

	bool startsWith(const std::string& str, const std::string& str2) {
		return str.substr(0, str2.size()) == str2;
	}

	std::string getFileNameWithoutExtension(const std::string& path)
	{
		FishEngine::Path p(path);
		return p.stem().string();
	}

	std::string getExtensionWithoutDot(const std::string& path)
	{
		FishEngine::Path p(path);
		return boost::to_lower_copy(p.extension().string().substr(1));
	}
#endif
}
