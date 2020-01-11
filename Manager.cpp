#include "Manager.h"

bool GetConsoleHandle(HANDLE& hConsole, const std::string& title)
{
	hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	if (hConsole == INVALID_HANDLE_VALUE) return FALSE;

	if (strcmp(title.c_str(), "")) if (!SetConsoleTitleA(title.c_str())) goto Error;
	if (!SetConsoleActiveScreenBuffer(hConsole)) goto Error;

	return TRUE;

Error:
	CloseHandle(hConsole);
	return FALSE;
}

void ConsoleManager::SetConsoleColors(COLOR_TYPE color_type)
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
		for (int i = 0; i < 16; ++i) {
			HSL c;
			c.hue = (i / 16.0) * 360;
			c.saturation = 50;
			c.luminance = 50;
			cv::Vec3b ret = HSLToRGB(c);

			csbi.ColorTable[i] = RGB(ret[0], ret[1], ret[2]);
		}
		break;
	}

	SetConsoleScreenBufferInfoEx(hConsole, &csbi);
}

bool ConsoleManager::SetConsoleFont(unsigned short fontSize, const std::string& fontName)
{
	CONSOLE_FONT_INFOEX cfie;
	ZeroMemory(&cfie, sizeof(cfie));
	cfie.cbSize = sizeof(cfie);
	cfie.dwFontSize.Y = fontSize;
	lstrcpyW(cfie.FaceName, (LPCWSTR)fontName.c_str());

	if (SetCurrentConsoleFontEx(hConsole, TRUE, &cfie)) return true;

	// Error
	CloseHandle(hConsole);
	return false;
}

bool ConsoleManager::SetConsoleSize(short w, short h)
{
	width = w;
	height = h;

	COORD largest, bufferSize;

	bufferRect = { 0, 0, 1, 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &bufferRect)) goto Error;

	largest = GetLargestConsoleWindowSize(hConsole);
	bufferSize = { std::min(largest.X, width), std::min(largest.Y, height) };
	if (!SetConsoleScreenBufferSize(hConsole, bufferSize)) goto Error;

	bufferRect = { 0, 0, bufferSize.X - 1, bufferSize.Y - 1 };
	if (!SetConsoleWindowInfo(hConsole, TRUE, &bufferRect)) goto Error;

	return true;

Error:	// Error
	CloseHandle(hConsole);
	return false;
}

bool ConsoleManager::Draw(const std::vector<uchar>& data, std::vector<uint>& colorData)
{
	buffer = std::vector<CHAR_INFO>(width * height);

	if (colorData.size() < data.size())
		for (int i = colorData.size(); i < data.size(); ++i)
			colorData.push_back(0);

	// Clear Part that is not recognized in data
	if (data.size() < buffer.size()) {
		for (int i = data.size(); i < buffer.size(); ++i) {
			buffer[i].Char.AsciiChar = ' ';
			buffer[i].Attributes = 0;
		}
	}

	for (std::vector<uchar>::size_type i = 0; i < data.size(); ++i) {
		buffer[i].Char.AsciiChar = data[i];
		buffer[i].Attributes = colorData[i];
	}

	if (!WriteConsoleOutputA(hConsole, &buffer[0], { width, height }, { 0, 0 }, &bufferRect)) std::cout << "WriteConsoleOutputA(): \n\n" << GetLastError() << std::endl;
	return true;
}

bool ConsoleManager::Draw(const std::vector<uchar>& data, std::vector<uint>& color_data, short w, short h, short font_size, COLOR_TYPE color_type)
{
	// INIT
	width = w;
	height = h;

	buffer = std::vector<CHAR_INFO>(width * height);

	// Colors
	SetConsoleColors(color_type);

	SMALL_RECT consoleWriteArea = { 0, 0, width - 1, height - 1 };
	SetConsoleFont(font_size, "Consolas");								// FONT SIZE
	SetConsoleSize(width, height);
	return true;
}

ConsoleManager::ConsoleManager() 
	: hConsole(0), buffer(), width(0), height(0), bufferRect({0, 0, 1, 1})
{
	GetConsoleHandle(hConsole, "Window");
}












//// INIT
//	width = w;
//	height = h;
//
//	buffer = std::vector<CHAR_INFO>(width * height);
//
//	//SMALL_RECT minimal = { 0, 0, 1, 1 };
//
//	/*if (!SetConsoleWindowInfo(hConsole, TRUE, &minimal)) std::cout << "SetConsoleWindow(): \n" << GetLastError() << std::endl;
//	if (!SetConsoleScreenBufferSize(hConsole, { width, height })) std::cout << "SetConsoleScreenBufferSize(): \n" << GetLastError() << std::endl;
//	SetConsoleActiveScreenBuffer(hConsole);*/
//
//	// Colors
//SetConsoleColors(color_type);
//
//SMALL_RECT consoleWriteArea = { 0, 0, width - 1, height - 1 };
//SetConsoleFont(font_size, "Consolas");								// FONT SIZE
//SetConsoleSize(width, height);
//
//// DRAW
//
//// Clear Buffer
//for (int i = 0; i < width * height; ++i) {
//	buffer[i].Char.AsciiChar = ' ';
//	buffer[i].Attributes = 0;
//}
//
//for (std::vector<uchar>::size_type i = 0; i < data.size(); ++i) {
//	buffer[i].Char.AsciiChar = data[i];
//	buffer[i].Attributes = color_data[i];
//}
//
//if (!WriteConsoleOutputA(hConsole, &buffer[0], { width, height }, { 0, 0 }, &consoleWriteArea)) std::cout << "WriteConsoleOutputA(): \n\n" << GetLastError() << std::endl;

