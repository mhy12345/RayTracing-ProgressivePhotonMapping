#include "object.h"
#include <random>
#include <glog/logging.h>
#include <cstdlib>
#include <iostream>

Object::Object() {
	hash = rand();
}

Object::~Object() {
}

void Material::accept(const Json::Value& val) {
	if (!val.isMember("refl"))
		LOG(ERROR)<<"No refl found in material..."<<std::endl;
	refl = val["refl"].asDouble();
	if (!val.isMember("spec"))
		LOG(ERROR)<<"No spec found in material..."<<std::endl;
	spec = val["spec"].asDouble();
	if (!val.isMember("diff"))
		LOG(ERROR)<<"No diff found in material..."<<std::endl;
	diff = val["diff"].asDouble();
}

void Object::accept(const Json::Value& val) {
	name = val["name"].asString();
	if (!val.isMember("color")) {
		LOG(ERROR)<<"No color info find"<<std::endl;
	}
	color.accept(val["color"]);
	material.accept(val["material"]);
}

std::string Collision::description()const {
	static char buf[20];
	sprintf(buf,"%f",dist);
	return std::string("Collision : \n") +
		"C = "+C.description() +"\n" +
		"N = " + N.description() + "\n" +
		"D = " + D.description() + "\n" + std::string(buf);
}
