#include "area_light.h"
#include "glog/logging.h"
#include <cmath>
#include <iostream>
#include <random>

void AreaLight :: accept(const Json::Value& val) {
	Light::accept(val);
	O.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
	LOG(INFO)<<"AreaLight : Data accepted."<<std::endl;
}

bool AreaLight :: collideWith(const Vector& rayO, const Vector& rayD) {
	assert(rayD.isUnit());
	LOG(INFO)<<"Area Light : "<<rayO.description()<<" "<<rayD.description()<<std::endl;
	Vector N = (dx*dy).unit();
	double d = -(N^O);
	double t = -(d+(N^rayO))/(N^rayD);
	if (!isfinite(t))return false;
	collision.dist = t;
	collision.C = rayO + t*rayD;
	double r1 = ((collision.C - O)^dx)/dx.sqrlen();
	double r2 = ((collision.C - O)^dy)/dy.sqrlen();
	if (r1>1-feps || r2>1-feps || r1<feps || r2<feps)
		return false;
	collision.N = N;
	collision.D = N*(rayD.reverse()^N)*2-rayD.reverse();
	LOG(INFO)<<"Area Light <"<<name<<">: hitted"<<std::endl;
	return true;
}

Vector AreaLight::getCenter()const {
	return O + dx/2 + dy/2;
}

double AreaLight::getShade(const Vector& _rayO,std::vector<Object*> olist, int shade_quality)const {
	LOG(INFO)<<"Calculate shade at <"<<_rayO.description()<<">"<<std::endl;
	Vector rayO = _rayO;
	int success_count = 0;
	for (int i=0;i<3;i++) {
		for (int j=0;j<3;j++) {
			for (int k=0;k<shade_quality;k++) {
				Vector checkO = rayO;
				Vector checkT = (O + dx/3*(i + rand()/RAND_MAX) + dy/3*(i + rand()/RAND_MAX));
				Vector checkD = (rayO - checkT).unit();
				double dist = (rayO - checkT).len();
				bool flag = true;
				for (auto &w: olist)
					if (w->collideWith(checkO,checkD) && w->getCollision().dist < dist) {
						flag = false;
						break;
				}
				if (flag)
					success_count ++;
			}
		}
	}
	double shade = success_count / (9*shade_quality);
	LOG(INFO)<<"Area Light <"<<name<<"> - shade : "<<shade<<std::endl;
	return shade;
}
