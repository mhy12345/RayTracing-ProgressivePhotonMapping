#include "vector.h"
#include <cmath>
#include <sstream>
#include <iostream>

Vector :: Vector() {
	x = 0 , y = 0, z = 0;
}

Vector :: Vector(double x,double y,double z) : x(x), y(y), z(z){
}

Vector :: Vector(const Eigen::Vector3d vec) : x(vec(0,0)),y(vec(1,0)),z(vec(2,0)) {
}

void Vector :: accept(const Json::Value& val) {
	if (!val.isMember("x") || !val.isMember("y") || !val.isMember("z"))
		std::cout<<"The vector not found..."<<std::endl;
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
Vector operator -(const Vector &a) {
	return Vector(-a.x,-a.y,-a.z);
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
bool Vector::isUnit()const {
	return abs(x*x+y*y+z*z - 1) < feps;
}

Vector each_max(const Vector& v1,const Vector& v2) {
	return Vector(std::max(v1.x,v2.x),std::max(v1.y,v2.y),std::max(v1.z,v2.z));
}

Vector each_min(const Vector& v1,const Vector& v2) {
	return Vector(std::min(v1.x,v2.x),std::min(v1.y,v2.y),std::min(v1.z,v2.z));
}

std::string Vector::description()const {
	std::stringstream ss;
	std::string res;
	ss<<"Vector<"<<x<<","<<y<<","<<z<<">";
	ss>>res;
	return res;
}
Eigen::Vector3d Vector::eigen()const {
	return Eigen::Vector3d(x,y,z);
}

Vector Vector::randomVectorOnSphere() {
	double theta = rand()*1.0/RAND_MAX*M_PI*2;
	double phi = rand()*1.0/RAND_MAX*M_PI*2 - M_PI;
	return Vector(cos(phi)*sin(theta),cos(phi)*cos(theta),sin(phi));
}
