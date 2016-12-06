#pragma once

#include <string>
#include <sstream>

class Convert
{
public:

	static inline std::string toString(char ch)
	{
		std::string str;
		str.push_back(ch);
		return str;
	}

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
