#include "light.h"
#include "glog/logging.h"
#include <iostream>

Light::Light() : Object() {
}
Light::~Light() {
}

void AreaLight :: accept(const Json::Value& val) {
	position.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
	color.accept(val["color"]);
	LOG(INFO)<<"AreaLight : Data accepted."<<std::endl;
}

void AreaLight :: collideWith(const Ray& ray) {
}
