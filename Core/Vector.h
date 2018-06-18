#ifndef VECTOR_H
#define VECTOR_H
#include "sceneloader.h"
#include "gobject.h"

class Vector : GObject
{
	double x,y,z;
	public:
		Vector();
		Vector(double x,double y,double z);
		double getX() const {return x;}
		double getY() const {return y;}
		double getZ() const {return z;}
		virtual void accept(SceneLoader sl);
		friend Vector XMul(const Vector &a,const Vector &b);
		friend double DMul(const Vector &a,const Vector &b);
		friend Vector operator + (const Vector &a,const Vector &b);
		friend Vector operator - (const Vector &a,const Vector &b);
		friend Vector operator * (double k,const Vector &a);
		friend bool operator == (const Vector &a,const Vector &b);
};
#endif
