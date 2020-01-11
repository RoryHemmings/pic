#pragma once

#ifndef MANAGER_H
#define MANAGER_H

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>

#include <Windows.h>

#include "constants.h"

typedef unsigned char uchar;

class ConsoleManager
{

public:
	static ConsoleManager& Get()
	{ 
		static ConsoleManager instance;
		return instance;
	}

	bool Draw(const std::vector<uchar>& data, std::vector<uint>& colorData);								/*
																														each element in data matches 1 to 1 with
																														element with same index in colorData the 
																														former being the character and the latter
																														being the color

																														(If the color data is shorter in size then
																														the buffer, then it will automatically be
																														padded with 0's

																														returns false if buffer and data differ in size
																													*/
	bool Draw(const std::vector<uchar>& data, std::vector<uint>& colorData, short w, short h, short font_size, COLOR_TYPE color_type=SOLID);

	void SetConsoleColors(COLOR_TYPE);
	bool SetConsoleFont(unsigned short fontSize, const std::string& fontName);
	bool SetConsoleSize(short w, short h);

private:
	ConsoleManager();

	HANDLE hConsole;
	std::vector<CHAR_INFO> buffer;
	SMALL_RECT bufferRect;

	short width, height;

};

#define CONSOLE_MANAGER ConsoleManager::Get()

#endif
