#include "texture.h"
#include <iostream>
using namespace cv;

Texture::Texture() {
}
Texture::~Texture() {
}

PureColorTexture::PureColorTexture() {
}
PureColorTexture::~PureColorTexture() {
}

Color PureColorTexture::getColor(double,double) {
	return color;
}

void PureColorTexture::accept(const Json::Value& val) {
	color.accept(val);
}

PictureTexture::PictureTexture() {
}
PictureTexture::~PictureTexture() {
}

Color PictureTexture::getColor(double x,double y) { 
	int ix = int (floor(fmod(x,rx)/rx * image.rows));
	int iy = int (floor(fmod(y,ry)/ry * image.cols));
	double r = image.at<Vec3b>(ix, iy)[0]/255.0;
	double g = image.at<Vec3b>(ix, iy)[1]/255.0;
	double b = image.at<Vec3b>(ix, iy)[2]/255.0;
	return Color(r,g,b);
}

void PictureTexture::accept(const Json::Value& val) {
	filename = val["filename"].asString();
	rx = val["rx"].asDouble();
	ry = val["ry"].asDouble();
	image = cv::imread(filename.c_str());
}
