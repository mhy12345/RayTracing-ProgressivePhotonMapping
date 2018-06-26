#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "../core/light.h"

class PointLight : public Light {
	private:
		Vector O;
	public:
		PointLight(){}
		void accept(const Json::Value& val);
		bool collideWith(const Vector& rayO, const Vector& rayD, Collision& collision);
		Vector getCenter()const;
		double getShade(const Vector& rayO,std::vector<Object*> olist,int shade_quality)const;
		void randomlyEmit(Vector& rayO,Vector& rayD)const;
};

#endif
