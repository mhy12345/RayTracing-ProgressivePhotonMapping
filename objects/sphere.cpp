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

const Color& Sphere::getColor()const {
	return color;
}

/*
 * ( Cir_O - (Ray_O + k*Ray_D) ).sqrlen() = r*r
 *
 */

bool Sphere::collideWith(const Vector& rayO,const Vector& rayD) {
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
		LOG(INFO)<<"Sphere <"<<name<<"> : hitted."<<std::endl;
		LOG(INFO)<<collision.description()<<std::endl;
		return true;
	}else {
		return false;
	}
}
