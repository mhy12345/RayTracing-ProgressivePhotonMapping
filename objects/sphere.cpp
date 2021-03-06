#include "sphere.h"
#include <cmath>
#include <cassert>
#include <iostream>

Sphere::Sphere() {
}

void Sphere::accept(const Json::Value& val) {
	Object::accept(val);
	O.accept(val["center"]);
	radius = val["radius"].asDouble();
}

Color Sphere::getColor(const Vector&v)const {
	if (texture->getType() == TEXTURE_PURE_COLOR)
	{
		return texture->getColor();
	}else if (texture->getType() == TEXTURE_PICTURE) {
		Vector tmp = (v - O)/radius;
		if (!tmp.isUnit())
			std::cout<<"The vector is not on the surface!"<<std::endl;
		double x = asin(tmp.getZ())+M_PI/2;
		double y = atan2(tmp.getX(),tmp.getY());
		Color res = texture->getColor(x,y);
		//std::cout<<res.description()<<std::endl;
		return res;
	}
	assert(false);
}

bool Sphere::collideWith(const Vector& rayO,const Vector& rayD,Collision& collision) {
	assert(rayD.isUnit());
	Vector V = O-rayO;
	double c = V.sqrlen()-radius*radius;
	double b = -(V^rayD)*2;
	double a = rayD.sqrlen();
	double delta = b*b-4*a*c;
	if (delta > feps) {
		double d1 = (-b-sqrt(delta)) / (2*a);
		double d2 = (-b+sqrt(delta)) / (2*a);
		collision.dist = d1;
		if (d1 < 0 && d2 < 0){
			return false;
		}else if (d1 < 0 && d2 >= 0) {
			collision.dist = d2;
			collision.face = false;
		}else {
			collision.dist = d1;
			collision.face = true;
		}
		collision.belongs = this;
		collision.C = rayO + rayD * collision.dist;
		if (collision.face)
			collision.N = (collision.C - O).unit();
		else
			collision.N = (O-collision.C).unit();
		Vector NN = collision.N * (collision.N ^ rayD.reverse());
		collision.I = rayD;
		return true;
	}else {
		return false;
	}
}
