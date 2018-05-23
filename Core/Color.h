#ifndef COLOR_H
#define COLOR_H

class Color
{
	double r,g,b;
	public:
		Color();
		Color(double r,double g,double b);
		friend Color operator +(const Color& a,const Color &b);
};

#endif
