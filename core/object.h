#ifndef OBJECT_H
#define OBJECT_H

#include "json/json.h"
#include "color.h"
#include "vector.h"
#include "ray.h"

struct Collision {
	Ray refl_ray;
	Vector N;
	double dist;
};

class Object {
	protected:
		unsigned hash;
		Collision collision;
	public:
		Object();
		virtual ~Object();
		virtual void accept(const Json::Value& val) = 0;
		virtual const Color& getColor()const = 0;
		unsigned getHash()const {return hash;}
		const Collision& getCollision() {return collision;}
		virtual bool collideWith(const Ray& ray) = 0;
};


#endif
