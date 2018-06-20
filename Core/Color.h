#ifndef COLOR_H
#define COLOR_H
#include "json/json.h"
#include <string>

class Color
{
	private:
		double r,g,b;
	public:
		Color();
		Color(double r,double g,double b);
		Color operator += (const Color&a);
		void accept(const Json::Value& val);
		double getR()const {return r;}
		double getG()const {return g;}
		double getB()const {return b;}
		friend Color operator +(const Color& a,const Color &b);
		friend Color operator *(const Color& a,double k);
		friend Color operator *(const Color& a,const Color& b);
		std::string description()const;
};

#endif
