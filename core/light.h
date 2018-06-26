#ifndef LIGHT_H
#define LIGHT_H

#include "object.h"

class Light : public Object {
	protected:
		double brightness;
	public:
		Light();
		virtual ~Light();
		virtual void accept(const Json::Value& val);
		virtual Vector getCenter()const = 0;
		virtual double getShade(const Vector& rayO,std::vector<Object*> olist,int shade_quality)const = 0;
		virtual void randomlyEmit(Vector& rayO,Vector& rayD)const = 0;
		double getBrightness()const {return brightness;}
		Color getColor(const Vector&)const;
};


#endif
