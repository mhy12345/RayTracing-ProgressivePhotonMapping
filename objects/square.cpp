#include "sphere.h"
#include <cmath>

Square::Square() {
}

void Square::accept(const Json::Value& val) {
	O.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
}

const Color& Square::getColor()const {
	return color;
}

/*
 * (Cir_O - Ray_O + Ray_D*k).sqrlen() = r*r
 */

bool Square::collideWith(const Ray& ray) {
}
