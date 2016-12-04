#pragma once

#include <string>
#include <sstream>

class Convert
{
public:
	static inline double toDouble(std::string str)
	{
		std::stringstream stream(str);
		double value;
		stream >> value;
		return value;
	}

	static inline int toInt(std::string str)
	{
		std::stringstream stream(str);
		int value;
		stream >> value;
		return value;
	}
};
