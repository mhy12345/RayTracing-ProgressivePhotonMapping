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
		virtual const Color& getColor()const;
		virtual bool collideWith(const Ray& ray);
};

#endif
