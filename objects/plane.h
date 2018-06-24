#ifndef PLANE_H
#define PLANE_H

#include "../core/object.h"

class Plane : public Object {
	private:
		Vector O;
		Vector dx,dy;
		bool border;
	public:
		Plane();
		Plane(Vector O,Vector dx,Vector dy);
		void accept(const Json::Value& val);
		Color getColor(const Vector&)const;
		bool collideWith(const Vector& rayO,const Vector& rayD,Collision& collision);
};

#endif
