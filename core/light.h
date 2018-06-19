#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "color.h"

class Light : Object {
	protected:
		Color color;
	public:
		Light();
		virtual ~Light();
		virtual void accept(const Json::Value& val) = 0;
		const Color& getColor() {
			return color;
		}
};

class AreaLight : public Light {
	private:
		Vector position;
		Vector dx,dy;
	public:
		AreaLight(){}
		void accept(const Json::Value& val);
		bool collideWith(const Ray& ray);
};

#endif
