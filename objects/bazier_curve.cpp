
#include "bazier_curve.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include "glog/logging.h"

BazierCurve::BazierCurve() {
}

void BazierCurve::accept(const Json::Value& val) {
	Object::accept(val);
	Q.accept(val["position"]);
	for (int i=0;i<4;i++)
	{
	   	px[i] = val["ctrl_pts"][i][0].asDouble();
		py[i] = val["ctrl_pts"][i][1].asDouble();
	}
}

Color BazierCurve::getColor(const Vector&)const {
	if (texture->getType() != TEXTURE_PURE_COLOR)
		DLOG(FATAL)<<"The getColor of BazierCurve only support PURE_COLOR_MODE"<<std::endl;
	return texture->getColor();
}

bool BazierCurve::collideWith(const Vector& rayO,const Vector& rayD,Collision& collision) {
	/*
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
		collision.D = 2*NN - rayD.reverse();
		DLOG(INFO)<<"BazierCurve <"<<name<<"> : hitted."<<std::endl;
		DLOG(INFO)<<collision.description()<<std::endl;
		return true;
	}else {
		return false;
	}*/
	assert(false);
	return false;
}
