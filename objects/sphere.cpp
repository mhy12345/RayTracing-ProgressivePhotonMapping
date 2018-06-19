#ifndef SPHERE_H
#define SPHERE_H

#include "../core/vector.h"
#include "../core/object.h"

class Square : public Object {
	private:
		Vector O;
		Vector dx,dy;
	public:
		Square();
		virtual void accept(const Json::Value& val);
		virtual const Color& getColor()const;
		virtual bool collideWith(const Ray& ray);
};

#endif
