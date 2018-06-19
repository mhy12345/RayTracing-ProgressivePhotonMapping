#include "color.h"
#include "glog/logging.h"

Color::Color() {
	r = g = b = 0;
}

Color::Color(double r,double g,double b) : r(r),g(g),b(b) {
}

Color operator +(const Color& a,const Color &b) {
	return Color(a.r+b.r,a.g+b.g,a.b+b.b);
}
void Color::accept(const Json::Value& val) {
	r = val["r"].asDouble();
	g = val["g"].asDouble();
	b = val["b"].asDouble();
	LOG(INFO)<<"Color : Data accepted."<<std::endl;
}
