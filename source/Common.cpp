#include "Common.hpp"

NAMESPACE_FISHENGINE_BEGIN

std::vector<std::string> split(const std::string& str, const std::string& separator){
    auto l = separator.length();
    auto pos = str.find_first_of(separator);
    decltype(pos) last_pos = 0;
    std::vector<std::string> ret;
    while (pos != std::string::npos) {
        ret.push_back(str.substr(last_pos, pos-last_pos));
        pos += l;
        last_pos = pos;
        pos = str.find_first_of(separator, pos);
    }
    ret.push_back(str.substr(last_pos, pos));
    return ret;
}

std::string camelCaseToReadable(const std::string& str)
{
    std::string ret;
    ret.reserve(str.size()*2); // at most: ceil(str.size()/2)*3
    ret.push_back(str[0]);
    for (size_t i = 1; i < str.size(); ++i) {
        char c = str[i];
        if (isupper(c) && !isupper(str[i-1]))
            ret.push_back(' ');
        ret.push_back(c);
    }
    ret.push_back(0);
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

NAMESPACE_FISHENGINE_END
