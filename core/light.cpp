#include "light.h"
#include "glog/logging.h"
#include <iostream>

Light::Light() : Object() {
}
Light::~Light() {
}

void Light::accept(const Json::Value& val) {
	Object::accept(val);
}
