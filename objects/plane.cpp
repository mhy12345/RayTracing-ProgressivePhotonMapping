#include "plane.h"
#include <cmath>
#include <cassert>
#include <iostream>
#include "glog/logging.h"

Plane::Plane() {
}

void Plane::accept(const Json::Value& val) {
	Object::accept(val);
	O.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
}

Color Plane::getColor(const Vector& pos)const {
	if (texture->getType() == TEXTURE_PURE_COLOR) {
		return texture->getColor();
	}else if (texture->getType() == TEXTURE_PICTURE) {
		double tx = ((pos - O)^dx)/dx.len();
		double ty = ((pos - O)^dy)/dy.len();
		return texture->getColor(tx,ty);
	}else
	DLOG(FATAL)<<"The getColor of Plane only support PURE_COLOR_MODE"<<std::endl;
}

bool Plane::collideWith(const Vector& rayO,const Vector& rayD) {
	assert(rayD.isUnit());
	DLOG(INFO)<<"Calc Collision : "<<rayO.description()<<" "<<rayD.description()<<std::endl;
	Vector N = (dx*dy).unit();
	double d = -(N^O);
	double t = -(d+(N^rayO))/(N^rayD);
	if (!isfinite(t))return false;
	if (t < feps) {
		if (abs(t) < feps) {
			DLOG(WARNING)<<"Plane <"<<name<<">: border ignored"<<std::endl;
		}
		return false;
	}
	collision.dist = t;
	collision.C = rayO + t*rayD;
	collision.face = (rayD^N) < 0;
	double r1 = ((collision.C - O)^dx)/dx.sqrlen();
	double r2 = ((collision.C - O)^dy)/dy.sqrlen();
	if (r1>1-feps || r2>1-feps || r1<feps || r2<feps)
		return false;
	collision.N = collision.face ? N : -N;
	collision.D = N*(rayD.reverse()^N)*2-rayD.reverse();
	DLOG(INFO)<<"Plane <"<<name<<">: hitted"<<std::endl;
	DLOG(INFO)<<collision.description()<<std::endl;
	return true;
}
