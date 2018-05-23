#include "Color.h"

Color::Color() {
	r = g = b = 0;
}

Color::Color(double r,double g,double b) : r(r),g(g),b(b) {
}

Color operator +(const Color& a,const Color &b) {
	return Color(a.r+b.r,a.g+b.g,a.b+b.b);
}
