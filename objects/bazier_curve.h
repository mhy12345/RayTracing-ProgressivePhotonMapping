#ifndef BAZIER_CURVE_H
#define BAZIER_CURVE_H

#include "../core/vector.h"
#include "../core/object.h"

class BazierCurve : public Object {
	private:
		Vector Q;
		Color color;
		double px[4];
		double py[4];
	public:
		BazierCurve();
		virtual void accept(const Json::Value& val);
		Color getColor(const Vector&)const;
		bool collideWith(const Vector& rayO,const Vector& rayD,Collision& collision);
};

#endif
