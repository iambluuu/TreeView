#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

inline float parametric(float time) {
	int alpha = 2;

	float left = 1.0f, right = 1.0f;
	while (alpha--) {
		left *= time;
		right *= (1.0f - time);
	}

	return left / (left + right);
}

inline void ReadQuotedString(std::stringstream& l_stream, std::string& l_string)
{
	l_stream >> l_string;
	if (l_string.at(0) == '"') {
		while (l_string.at(l_string.length() - 1) != '"' || !l_stream.eof())
		{
			std::string str;
			l_stream >> str;
			l_string.append(" " + str);
		}
	}
	l_string.erase(std::remove(l_string.begin(), l_string.end(), '"'), l_string.end());
}



