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

bool Plane::collideWith(const Vector& rayO,const Vector& rayD) {
	assert(rayD.isUnit());
	LOG(INFO)<<"Calc Collision : "<<rayO.description()<<" "<<rayD.description()<<std::endl;
	Vector N = (dx*dy).unit();
	double d = -(N^O);
	double t = -(d+(N^rayO))/(N^rayD);
	if (!isfinite(t))return false;
	if (t < feps) {
		if (abs(t) < feps) {
			LOG(WARNING)<<"Plane <"<<name<<">: border ignored"<<std::endl;
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
	LOG(INFO)<<"Plane <"<<name<<">: hitted"<<std::endl;
	LOG(INFO)<<collision.description()<<std::endl;
	return true;
}
