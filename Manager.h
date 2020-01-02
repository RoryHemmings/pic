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

static BOOL GetConsoleHandle(HANDLE& hConsole, LPCSTR szTitle) {
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hConsole == INVALID_HANDLE_VALUE) return FALSE;

	if (strcmp(szTitle, "")) if (!SetConsoleTitleA(szTitle)) goto Error;
	if (!SetConsoleActiveScreenBuffer(hConsole)) goto Error;

	return TRUE;

Error:
	CloseHandle(hConsole);
	return FALSE;
}

static BOOL SetConsoleSize(HANDLE& hConsole, SHORT nScreenWidth, SHORT nScreenHeight, SMALL_RECT& srBufferRect) {
	COORD largest, bufferSize;

	srBufferRect = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &srBufferRect)) goto Error;

	largest = GetLargestConsoleWindowSize(hConsole);
	bufferSize = { std::min(largest.X, nScreenWidth), std::min(largest.Y, nScreenHeight) };
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) goto Error;

	srBufferRect = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &srBufferRect)) goto Error;

	return TRUE;

Error:	// Error
	CloseHandle(hConsole);
	return FALSE;
}

static BOOL SetConsoleFont(HANDLE& hConsole, SHORT nFontSize, const CHAR* szFontName) {
	CONSOLE_FONT_INFOEX cfie;
	ZeroMemory(&cfie, sizeof(cfie));
	cfie.cbSize = sizeof(cfie);
	cfie.dwFontSize.Y = nFontSize;
	lstrcpyW(cfie.FaceName, (LPCWSTR)szFontName);

	if (SetCurrentConsoleFontEx(hConsole, TRUE, &cfie)) return TRUE;

	// Error
	CloseHandle(hConsole);
	return FALSE;
}

static VOID SetConsoleColors(HANDLE& hConsole, COLOR_TYPE color_type)
{
	CONSOLE_SCREEN_BUFFER_INFOEX csbi;
	csbi.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
	GetConsoleScreenBufferInfoEx(hConsole, &csbi);

	switch (color_type)
	{

	case SOLID:
		csbi.ColorTable[15] = RGB(255, 255, 255);
		break;
	
	case GRAYSCALE:
		for (int i = 0; i < 16; ++i) {
			csbi.ColorTable[i] = RGB(i * (255 / 16), i * (255 / 16), i * (255 / 16));
		}
		csbi.ColorTable[0] = 0;
		break;

	case COLOR:
		break;

	}

	SetConsoleScreenBufferInfoEx(hConsole, &csbi);
}

class Manager
{

public:
	static Manager& Get()
	{ 
		static Manager instance;
		return instance;
	}

	void draw(const std::vector<uchar>& data, const std::vector<uint>& color_data, SHORT w, SHORT h, SHORT font_size, COLOR_TYPE color_type=SOLID)
	{
		// INIT
		width = w;
		height = h;

		GetConsoleHandle(hConsole, "Window");
		buffer = std::vector<CHAR_INFO>(width * height);

		SMALL_RECT minimal = { 0, 0, 1, 1 };

		if (!SetConsoleWindowInfo(hConsole, TRUE, &minimal)) std::cout << "SetConsoleWindow(): \n" << GetLastError() << std::endl;
		if (!SetConsoleScreenBufferSize(hConsole, { width, height })) std::cout << "SetConsoleScreenBufferSize(): \n" << GetLastError() << std::endl;
		SetConsoleActiveScreenBuffer(hConsole);

		// Colors
		SetConsoleColors(hConsole, color_type);

		SMALL_RECT consoleWriteArea = { 0, 0, width - 1, height - 1 };
		SetConsoleFont(hConsole, font_size, "Consolas");								// FONT SIZE
		SetConsoleSize(hConsole, width, height, consoleWriteArea);

		// DRAW

		// Clear Buffer
		for (int i = 0; i < width * height; ++i) {
			buffer[i].Char.AsciiChar = ' ';
			buffer[i].Attributes = 0;
		}

		for (std::vector<uchar>::size_type i = 0; i < data.size(); ++i) {
			buffer[i].Char.AsciiChar = data[i];
			buffer[i].Attributes = color_data[i];
		}

		if (!WriteConsoleOutputA(hConsole, &buffer[0], { width, height }, { 0, 0 }, &consoleWriteArea)) std::cout << "WriteConsoleOutputA(): \n\n" << GetLastError() << std::endl;
	}

private:
	Manager() : hConsole(NULL), buffer(), width(0), height(0) { }

	HANDLE hConsole;
	std::vector<CHAR_INFO> buffer;

	SHORT width, height;

};

#define CONSOLE_MANAGER Manager::Get()

#endif
