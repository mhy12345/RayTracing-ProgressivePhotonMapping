#ifndef VECTOR_H
#define VECTOR_H
class Vector
{
	double x,y,z;
	public:
		Vector();
		Vector(double x,double y,double z);
		friend Vector XMul(const Vector &a,const Vector &b);
		friend double DMul(const Vector &a,const Vector &b);
		friend Vector operator + (const Vector &a,const Vector &b);
		friend Vector operator - (const Vector &a,const Vector &b);
		friend Vector operator * (double k,const Vector &a);
		friend bool operator == (const Vector &a,const Vector &b);
};
#endif
