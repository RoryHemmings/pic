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

	Picture p("reub.png", ASCII_PIC, 0.5, 0.5, GRAYSCALE);
	show(p);
	waitKey(0);

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
*/
