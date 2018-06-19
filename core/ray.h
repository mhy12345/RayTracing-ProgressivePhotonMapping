#ifndef RAY_H
#define RAY_H

#include "vector.h"

class Ray {
	public:
		Vector O;
		Vector D;
		Ray(){}
		Ray(Vector O,Vector D):O(O),D(D){}
};

#endif
