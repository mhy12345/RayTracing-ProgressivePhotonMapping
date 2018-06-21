#ifndef AREA_LIGHT_H
#define AREA_LIGHT_H

#include "../core/light.h"

class AreaLight : public Light {
	private:
		Vector O;
		Vector dx,dy;
	public:
		AreaLight(){}
		void accept(const Json::Value& val);
		bool collideWith(const Vector& rayO, const Vector& rayD, Collision& collision);
		Vector getCenter()const;
		double getShade(const Vector& rayO,std::vector<Object*> olist,int shade_quality)const;
};

#endif
