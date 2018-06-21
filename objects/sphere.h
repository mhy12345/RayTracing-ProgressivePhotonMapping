#ifndef SPHERE_H
#define SPHERE_H

#include "../core/vector.h"
#include "../core/object.h"

class Sphere : public Object {
	private:
		Vector O;
		double radius;
		Color color;
	public:
		Sphere();
		virtual void accept(const Json::Value& val);
		Color getColor(const Vector&)const;
		bool collideWith(const Vector& rayO,const Vector& rayD,Collision& collision);
};

#endif
