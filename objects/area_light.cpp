#include "area_light.h"
#include <cmath>
#include <iostream>
#include <random>

void AreaLight :: accept(const Json::Value& val) {
	Light::accept(val);
	O.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
}

bool AreaLight :: collideWith(const Vector& rayO, const Vector& rayD,Collision& collision) {
	assert(rayD.isUnit());
	Vector N = (dx*dy).unit();
	double d = -(N^O);
	double t = -(d+(N^rayO))/(N^rayD);
	if (!std::isfinite(t))return false;
	if (t<feps)return false;
	collision.belongs = this;
	collision.dist = t;
	collision.C = rayO + t*rayD;
	collision.face = (rayD^N) < 0;
	double r1 = ((collision.C - O)^dx)/dx.sqrlen();
	double r2 = ((collision.C - O)^dy)/dy.sqrlen();
	if (r1>1-feps || r2>1-feps || r1<feps || r2<feps)
		return false;
	collision.N = collision.face ? N : -N;
	collision.I = rayD;
	return true;
}

Vector AreaLight::getCenter()const {
	return O + dx/2 + dy/2;
}

double AreaLight::getShade(const Vector& _rayO,std::vector<Object*> olist, int shade_quality)const {
	Vector rayO = _rayO;
	int success_count = 0;
	for (int i=0;i<3;i++) {
		for (int j=0;j<3;j++) {
			for (int k=0;k<shade_quality;k++) {
				Vector checkO = rayO;
				Vector checkT = (O + dx/3*(i + rand()*1.0/RAND_MAX) + dy/3*(j + rand()*1.0/RAND_MAX));
				Vector checkD = (checkT - rayO).unit();
				double dist = (checkT - rayO).len();
				bool flag = true;
				for (auto &w: olist)
				{
					bool flg;
					Collision obj_coll;
					if ((flg = w->collideWith(checkO,checkD,obj_coll)) && obj_coll.dist < dist) {
						flag = false;
						break;
					}
				}
				if (flag)
					success_count ++;
			}
		}
	}
	double shade = success_count / (9.0*shade_quality);
	//if (success_count)shade = 1;
	return shade;
}
