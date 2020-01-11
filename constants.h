#pragma once

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <opencv2/opencv.hpp>
#include <algorithm>

enum PICTURE_TYPE
{
	ASCII_PIC, CANVAS_PIC
};

enum COLOR_TYPE
{
	COLOR, GRAYSCALE, SOLID
};

struct HSL
{
	uchar hue;
	uchar saturation;
	uchar luminance;
};

static double Min(double fR, double fG, double fB)
{
	double fMin = fR;
	if (fG < fMin)
	{
		fMin = fG;
	}
	if (fB < fMin)
	{
		fMin = fB;
	}
	return fMin;
}


static double Max(double fR, double fG, double fB)
{
	double fMax = fR;
	if (fG > fMax)
	{
		fMax = fG;
	}
	if (fB > fMax)
	{
		fMax = fB;
	}
	return fMax;
}

static uint HSLToColorIndex(HSL color)
{
	return (uint)floor((double)(color.hue / 361.0) * 16);
}

static HSL RGBToHSL(uint red, uint green, uint blue)
{
	HSL ret = { 0 };

	double fR = red / 255.0;
	double fG = green / 255.0;
	double fB = blue / 255.0;

	double fCMin = Min(fR, fG, fB);
	double fCMax = Max(fR, fG, fB);


	ret.luminance = (uchar)floor(50 * (fCMin + fCMax));	// Explicit casting to stop warning messages from clogging the console

	if (fCMin == fCMax) {
		ret.saturation = 0;
		ret.hue = 0;
		return ret;
	}
	else if (ret.luminance < 50) {
		ret.saturation = (uchar)(100 * (fCMax - fCMin) / (fCMax + fCMin));
	}
	else {
		ret.saturation = (uchar)(100 * (fCMax - fCMin) / (2.0 - fCMax - fCMin));
	}

	if (fCMax == fR) {
		ret.hue = (uchar)(60 * (fG - fB) / (fCMax - fCMin));
	}
	if (fCMax == fG) {
		ret.hue = (uchar)(60 * (fB - fR) / (fCMax - fCMin) + 120);
	}
	if (fCMax == fB) {
		ret.hue = (uchar)(60 * (fR - fG) / (fCMax - fCMin) + 240);
	}
	if (ret.hue < 0) {
		ret.hue = (uchar)(ret.hue + 360);
	}

	return ret;
}

static void HSLtoRGB_aux(uint& c, const double& temp1, const double& temp2, const double& temp3)
{
	if ((temp3 * 6) < 1)
		c = (uint)((temp2 + (temp1 - temp2) * 6 * temp3) * 100);
	else
		if ((temp3 * 2) < 1)
			c = (uint)(temp1 * 100);
		else
			if ((temp3 * 3) < 2)
				c = (uint)((temp2 + (temp1 - temp2) * (.66666 - temp3) * 6) * 100);
			else
				c = (uint)(temp2 * 100);
	return;
}

static cv::Vec3b HSLToRGB(const HSL& in)
{
	uint r = 0;
	uint g = 0;
	uint b = 0;

	double L = ((double)in.luminance) / 100;
	double S = ((double)in.saturation) / 100;
	double H = ((double)in.hue) / 360;

	if (in.saturation == 0) {
		r = in.luminance;
		g = in.luminance;
		b = in.luminance;
	}
	else {
		double temp1 = 0;
		if (L < 0.50) {
			temp1 = L * (1 + S);
		}
		else {
			temp1 = L + S - (L * S);
		}

		double temp2 = 2 * L - temp1;

		double temp3 = 0;
		for (int i = 0; i < 3; i++)
		{
			switch (i)
			{
			case 0:
				temp3 = H + 0.33333;
				if (temp3 > 1)
					temp3 -= 1;
				HSLtoRGB_aux(r, temp1, temp2, temp3);
				break;
			case 1:
				temp3 = H;
				HSLtoRGB_aux(g, temp1, temp2, temp3);
				break;
			case 2:
				temp3 = H - 0.33333;
				if (temp3 < 0)
					temp3 += 1;
				HSLtoRGB_aux(b, temp1, temp2, temp3);
				break;
			}
		}
	}

	uchar red = (uchar)((((double)r) / 100) * 255);
	uchar green = (uchar)((((double)g) / 100) * 255);
	uchar blue = (uchar)((((double)b) / 100) * 255);

	return { red, green, blue };
}

#endif
