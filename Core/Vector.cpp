#include "vector.h"
#include "sceneloader.h"

Vector :: Vector() {
	x = 0 , y = 0, z = 0;
}


Vector :: Vector(double x,double y,double z) : x(x), y(y), z(z){
}

void Vector :: accept(SceneLoader sl) {
	sl("ADDRESS") >> x >> y >> z;
}

Vector XMul(const Vector &a,const Vector &b){
	return Vector(a.y*b.z - a.z*b.y,a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

double DMul(const Vector &a,const Vector &b){
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector operator +(const Vector &a,const Vector &b) {
	return Vector(a.x+b.x,a.y+b.y,a.z+b.z);
}

Vector operator -(const Vector &a,const Vector &b) {
	return Vector(a.x-b.x,a.y-b.y,a.z-b.z);
}

Vector operator *(double k,const Vector &a) {
	return Vector(k*a.x,k*a.y,k*a.z);
}

bool operator ==(const Vector &a,const Vector &b) {
	return a.x == b.x && a.y == b.y && a.z == b.z;
}
