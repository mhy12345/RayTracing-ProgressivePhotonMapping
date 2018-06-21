#include "sphere.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include "glog/logging.h"

Sphere::Sphere() {
}

void Sphere::accept(const Json::Value& val) {
	Object::accept(val);
	O.accept(val["center"]);
	radius = val["radius"].asDouble();
	color.accept(val["color"]);
}

Color Sphere::getColor(const Vector&)const {
	if (texture->getType() != TEXTURE_PURE_COLOR)
		DLOG(FATAL)<<"The getColor of Sphere only support PURE_COLOR_MODE"<<std::endl;
	return texture->getColor();
}

bool Sphere::collideWith(const Vector& rayO,const Vector& rayD,Collision& collision) {
	assert(rayD.isUnit());
	Vector V = O-rayO;
	double c = V.sqrlen()-radius*radius;
	double b = -(V^rayD)*2;
	double a = rayD.sqrlen();
	double delta = b*b-4*a*c;
	if (delta > feps) {
		collision.dist = (-b-sqrt(delta)) / (2*a);
		if (collision.dist < feps) {
			return false;
		}
		collision.C = rayO + rayD * collision.dist;
		collision.N = (collision.C - O).unit();
		Vector NN = collision.N * (collision.N ^ rayD.reverse());
		collision.D = 2*NN - rayD.reverse();
		DLOG(INFO)<<"Sphere <"<<name<<"> : hitted."<<std::endl;
		DLOG(INFO)<<collision.description()<<std::endl;
		return true;
	}else {
		return false;
	}
}
