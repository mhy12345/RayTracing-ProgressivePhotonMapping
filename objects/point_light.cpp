#include "point_light.h"
#include <cmath>
#include <iostream>
#include <random>

void PointLight :: accept(const Json::Value& val) {
	Light::accept(val);
	O.accept(val["position"]);
}

bool PointLight :: collideWith(const Vector& rayO, const Vector& rayD,Collision& collision) {
	return false;
}

Vector PointLight::getCenter()const {
	return O;
}

double PointLight::getShade(const Vector& _rayO,std::vector<Object*> olist, int shade_quality)const {
	Vector rayO = _rayO;
	int success_count = 0;
	Vector checkO = rayO;
	Vector checkT = O;
	Vector checkD = (checkT - rayO).unit();
	double dist = (checkT - rayO).len();
	bool flag = true;
	for (auto &w: olist)
	{
		bool flg;
		Collision obj_coll;
		if ((flg = w->collideWith(checkO,checkD,obj_coll)) && obj_coll.dist < dist) {
			return 0;
		}
	}
	return 1;
}
void PointLight::randomlyEmit(Vector& rayO,Vector& rayD)const{
	rayO = O;
	rayD = Vector::randomVectorOnSphere();
}
