#include "Picture.h"
#include "gstc.h"

using namespace cv;
using namespace std;

ushort RGBToGrayscale(uint red, uint green, uint blue)
{
	return (red + green + blue) / 3;
}

uint RGBToColorIndex(uint red, uint green, uint blue)
{
	return HSLToColorIndex(RGBToHSL(red, green, blue));
}

void ManageColors(const Mat& img, vector<uint>& out, COLOR_TYPE color_type, uint width, uint height)
{
	uint len = width * height;
	out = {};
	switch (color_type)
	{

	case SOLID:
		out = vector<uint>(len, 15);
		break;

	case GRAYSCALE:
		for (uint i = 0; i != len; ++i) {
			Vec3b temp = img.at<Vec3b>(floor((int)(i / width)), i % width);
			out.push_back(ceil((RGBToGrayscale(temp[2], temp[1], temp[0]) / 256.0) * 15));
		}
		break;

	case COLOR:
		for (uint i = 0; i != len; ++i) {
			Vec3b temp = img.at<Vec3b>(floor((int)(i / width)), i % width);
			out.push_back(RGBToColorIndex(temp[2], temp[1], temp[0]));
		}
	}
}

void APic::create(const string& path, double scaleX, double scaleY, COLOR_TYPE color_type)
{

	Mat ref = imread(path);	// Load as color image
	if (ref.empty()) { throw "Image Couldn't load"; return; }

	Mat img;

	if (scaleX == 0 || scaleY == 0) {
		// Auto scale
	}

	resize(ref, img, Size(0, 0), scaleX, scaleY / 2);
	width = img.cols;
	height = img.rows;

	data = {};
	for (uint row = 0; row < height; ++row) {
		for (uint col = 0; col < width; ++col) {
			Vec3b temp = img.at<Vec3b>(row, col);
			data.push_back(gstc::convert(RGBToGrayscale(temp[2], temp[1], temp[0])));	// .at() returns BGR instead of RGB
		}
	}

	ManageColors(img, color_data, color_type, width, height);
}

void CPic::create(const string& path, double scaleX, double scaleY, COLOR_TYPE color_type)
{

}

void Pic::display() const
{
	//CONSOLE_MANAGER.Draw(data, color_data, width, height, 8, ct);
}

Picture::Picture(const string& path, PICTURE_TYPE type, COLOR_TYPE color_type)
{
	switch (type)
	{

	case ASCII_PIC:
		p = shared_ptr<Pic>(new APic(path, 0, 0, color_type));
		break;

	case CANVAS_PIC:
		p = shared_ptr<Pic>(new CPic(path, 0, 0, color_type));
		break;

	}
}

Picture::Picture(const string& path, PICTURE_TYPE type, double scaleX, double scaleY, COLOR_TYPE color_type)
{
	switch (type)
	{

	case ASCII_PIC:
		p = shared_ptr<Pic>(new APic(path, scaleX, scaleY, color_type));
		break;

	case CANVAS_PIC:
		p = shared_ptr<Pic>(new CPic(path, scaleX, scaleY, color_type));
		break;

	}
}

void show(const Picture& pic)
{
	pic.p->display();
}

