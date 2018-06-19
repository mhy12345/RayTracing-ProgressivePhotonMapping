#include "vector.h"
#include <cmath>

Vector :: Vector() {
	x = 0 , y = 0, z = 0;
}


Vector :: Vector(double x,double y,double z) : x(x), y(y), z(z){
}

void Vector :: accept(const Json::Value& val) {
	x = val["x"].asDouble();
	y = val["y"].asDouble();
	z = val["z"].asDouble();
}

Vector operator*(const Vector &a,const Vector &b){
	return Vector(a.y*b.z - a.z*b.y,a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

double operator^(const Vector &a,const Vector &b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector operator +(const Vector &a,const Vector &b) {
	return Vector(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vector operator -(const Vector &a,const Vector &b) {
	return Vector(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vector operator *(double k,const Vector &a) {
	return Vector(k*a.x,k*a.y,k*a.z);
}

Vector operator *(const Vector &a,double k) {
	return Vector(k*a.x,k*a.y,k*a.z);
}

Vector operator /(const Vector&a, double k) {
	return Vector(a.x/k,a.y/k,a.z/k);
}


bool operator ==(const Vector &a,const Vector &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}

double Vector::len()const {
	return sqrt(x*x+y*y+z*z);
}
double Vector::sqrlen()const {
	return x*x+y*y+z*z;
}
Vector Vector::unit()const {
	return (*this)/len();
}
Vector Vector::reverse()const {
	return Vector(-x,-y,-z);
}
