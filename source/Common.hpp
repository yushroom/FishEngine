#ifndef Common_hpp
#define Common_hpp

#include "FishEngine.hpp"

#include <string>
#include <vector>

NAMESPACE_FISHENGINE_BEGIN

//========== string ==========//

std::vector<std::string> split(const std::string& str, const std::string& separator);

std::string camelCaseToReadable(const std::string& str);

//void trim(std::string& str);
std::string trim(const std::string& str);

bool startsWith(const std::string& str, const std::string& str2);

NAMESPACE_FISHENGINE_END

#endif
