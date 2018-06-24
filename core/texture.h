#ifndef TEXTURE_H
#define TEXTURE_H

#include "color.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#define TEXTURE_PURE_COLOR 1
#define TEXTURE_PICTURE 2

class Texture
{
	public:
		Texture();
		virtual ~Texture();
		virtual Color getColor(double dx=0,double dy=0)const = 0;
		virtual void accept(const Json::Value& val) = 0;
		virtual int getType() = 0;
};

class PureColorTexture : public Texture {
	private:
		Color color;
	public:
		PureColorTexture();
		~PureColorTexture();
		Color getColor(double,double)const;
		int getType(){return TEXTURE_PURE_COLOR;}
		void accept(const Json::Value& val);
};

class PictureTexture : public Texture {
	private:
		std::string filename;
		double rx,ry;
		cv::Mat image;
	public:
		PictureTexture();
		~PictureTexture();
		Color getColor(double,double)const;
		int getType(){return TEXTURE_PICTURE;}
		void accept(const Json::Value& val);
};
#endif
