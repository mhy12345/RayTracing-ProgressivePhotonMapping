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
Color Light::getColor(const Vector&)const {
	if (texture->getType() != TEXTURE_PURE_COLOR)
		DLOG(FATAL)<<"The getColor of Light only support PURE_COLOR_MODE"<<std::endl;
	return texture->getColor();
}
