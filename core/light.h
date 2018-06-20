#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include "color.h"
#include "object.h"

class Light : public Object {
	protected:
	public:
		Light();
		virtual ~Light();
		virtual void accept(const Json::Value& val);
		virtual Vector getCenter()const = 0;
		virtual double getShade(const Vector& rayO,std::vector<Object*> olist,int shade_quality)const = 0;
		Color getColor(const Vector&)const;
};


#endif
