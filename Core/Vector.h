#ifndef VECTOR_H
#define VECTOR_H
#include "json/json.h"
#include <string>
#include <Eigen/Dense>

#define INF 0x3f3f3f3f
#define inf 1e100
#define feps 1e-8

class Vector
{
	double x,y,z;
	public:
		Vector();
		Vector(double x,double y,double z);
		Vector(const Eigen::Vector3d vec);
		double getX() const {return x;}
		double getY() const {return y;}
		double getZ() const {return z;}
		virtual void accept(const Json::Value &sl);
		friend Vector operator * (const Vector &a,const Vector &b);
		friend double operator ^ (const Vector &a,const Vector &b);
		friend Vector operator + (const Vector &a,const Vector &b);
		friend Vector operator - (const Vector &a,const Vector &b);
		friend Vector operator - (const Vector &a);
		friend Vector operator * (double k,const Vector &a);
		friend Vector operator * (const Vector &a,double k);
		friend bool operator == (const Vector &a,const Vector &b);
		friend Vector operator /(const Vector& a,double k);
		Vector unit()const;
		Vector reverse()const;
		double len()const;
		double sqrlen()const;
		bool isUnit()const;
		std::string description()const;
		Eigen::Vector3d eigen()const;
};
#endif
