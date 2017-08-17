#pragma once

#include <sstream>

// we need nicely formated version
// like 0.000 -> 0, 0.1234567890 -> 0.1234568, 18.0000001 -> 18
// std::to_sting(float) do not satisfy because it simply calls sprintf without format controlls
inline std::string floatToStdString(const float f)
{
	std::ostringstream ss;
	ss << f;
	return ss.str();
}
