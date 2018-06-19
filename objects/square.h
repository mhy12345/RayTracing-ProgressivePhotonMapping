#include "square.h"
#include <cmath>

Square::Square() {
}
void Square::accept(const Json::Value& val) {
	O.accept(val["center"]);
	radius = val["radius"].asDouble();
}

const Color& Square::getColor()const {
	return color;
}

/*
 * (Cir_O - Ray_O + Ray_D*k).sqrlen() = r*r
 */

bool Square::collideWith(const Ray& ray) {
	Vector V = O-ray.O;
	double a = ray.D.sqrlen();
	double b = ray.D^V;
	double c = V.sqrlen() - radius*radius;
	double det = b*b-4*a*c;
	if (det>eps) {
		double k = (-b-sqrt(det))/(2*a);
		collision.dist = k;//Make sure ray moduled 1
		Vector A = ray.D.reverse();
		Vector N = (collision.refl_ray.O - O).unit();
		collision.dist = k;
		N = N*(A^N);

		collision.refl_ray.O = k*ray.D + ray.O;
		collision.refl_ray.D = 2*N - A;
		return true;
	}else {
		return false;
	}
}
