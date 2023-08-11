#pragma once
#define RUNNING_WINDOWS
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>

#include <windows.h>
#include <commdlg.h>
#include <shlobj.h>

HWND hwnd;
TCHAR szFileName[MAX_PATH];

inline float parametric(float time) {
	int alpha = 2;

	float left = 1.0f, right = 1.0f;
	while (alpha--) {
		left *= time;
		right *= (1.0f - time);
	}

	return left / (left + right);
}

inline std::string BrowseString() {
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
	ofn.lpstrDefExt = L"txt";

	std::string s;

	if (GetOpenFileName(&ofn) == TRUE) {
		// User selected a file
		std::ifstream file(ofn.lpstrFile);
		std::string line;
		while (std::getline(file, line)) {
			s.append(line);
		}

		file.close();
	}
	else {
		// User cancelled the dialog
	}

	return s;
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



