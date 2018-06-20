#ifndef PLANE_H
#define PLANE_H

#include "../core/object.h"

class Plane : public Object {
	private:
		Vector O;
		Vector dx,dy;
		Color color;
	public:
		Plane();
		virtual void accept(const Json::Value& val);
		bool collideWith(const Vector& rayO,const Vector& rayD);
};

#endif
