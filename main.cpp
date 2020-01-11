#include <iostream>
#include <string>
#include <vector>

#include <Windows.h>
#include <opencv2/opencv.hpp>

#include "Picture.h"
#include "Manager.h"

#define DEBUGGING true
#define HELP "Help Message"

using namespace std;
using namespace cv;

int main(int argc, char** argv)
{
	if (!DEBUGGING) {
		if (argc == 1) {
			cout << HELP << endl;
			return 1;
		}
	}

	vector<uint> colorData = { 1, 2, 3, 4, 5, 6 };

	CONSOLE_MANAGER.SetConsoleFont(12, "consolas");
	CONSOLE_MANAGER.SetConsoleColors(COLOR);
	CONSOLE_MANAGER.SetConsoleSize(120, 30);
	if (!CONSOLE_MANAGER.Draw({ 'a', 'b', 'c', 'd', 'e', 'f', 'g' }, colorData)) cout << "fail" << endl;

	while (!GetAsyncKeyState(VK_SPACE)) {
		Sleep(5);
	}

	return 0;
}

/*
	TODO:
		compressed, and not
		color array
		shifting through scales
		adjust font size
		settings bar
		inverted font and ascii colors
		rehaul all colors to uchars

		reform picture class
		reform manager class
		rewrite names to be understandable
		use 3d distance
		dithering
		add cleanup
		stop using namespaces
*/
