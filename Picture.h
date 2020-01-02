#pragma once

#ifndef PICTURE_H
#define PICTURE_H

#include <vector>
#include <string>
#include <memory>

#include <Windows.h>

#include <opencv2/opencv.hpp>

#include "Manager.h"
#include "constants.h"

using namespace cv;

class Picture;

class Pic
{

friend class APic;
friend class CPic;

friend void show(const Picture&);

private:
	Pic() : width(0), height(0), data(), ct(SOLID) { }
	Pic(COLOR_TYPE c) : width(0), height(0), data(), ct(c) { }
	virtual ~Pic() { }

	void display() const;
	virtual void create(const std::string&, double, double, COLOR_TYPE) = 0;

protected:
	uint width, height;	// In characters
	std::vector<uchar> data;
	std::vector<uint> color_data;
	COLOR_TYPE ct;

};

class APic : public Pic
{
	friend class Picture;

	APic(const std::string& path, double scaleX, double scaleY, COLOR_TYPE color_type) : Pic(color_type) { create(path, scaleX, scaleY, color_type); }	
																								// Making scaleX or scaleY 0 causes automatic scaling for that axis
																								// Leaving 0 for both causes complete automatic scaling to fit screen

	void create(const std::string&, double, double, COLOR_TYPE);	/*
																		1. Create reference matrix
																		2. Create scaled version of that matrix
																		3. Iterate through image data
																			- Set vector position to character that matches in intensity
																		4. Check color type
																			- Iterate through image data and match colors to corresponding color index
																	*/
};

class CPic : public Pic
{
	friend class Picture;

	CPic(const std::string& path, double scaleX, double scaleY, COLOR_TYPE color_type) : Pic(color_type) { create(path, scaleX, scaleY, color_type); }		
																								// Making scaleX or scaleY 0 causes automatic scaling for that axis
																								// Leaving 0 for both causes complete automatic scaling to fit screen

	void create(const std::string&, double, double, COLOR_TYPE);	/*
																		1. Create reference matrix
																		2. Create scaled version of that matrix
																		3. Check color type
																			- Iterate through image data and match colors to corresponding color index
																	*/
};

class Picture
{

friend void show(const Picture&);

public:
	Picture() : p(0) { }
	Picture(std::shared_ptr<Pic> p) : p(p) { }
	explicit Picture(const std::string&, PICTURE_TYPE, COLOR_TYPE);		// Auto-scaling
	explicit Picture(const std::string&, PICTURE_TYPE, double, double, COLOR_TYPE);		// Manual-scaling

private:
	std::shared_ptr<Pic> p;

};

void show(const Picture&);

#endif
