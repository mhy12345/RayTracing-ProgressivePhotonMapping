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
		DLOG(ERROR)<<"No refl found in material..."<<std::endl;
	refl = val["refl"].asDouble();
	if (!val.isMember("spec"))
		DLOG(ERROR)<<"No spec found in material..."<<std::endl;
	spec = val["spec"].asDouble();
	if (!val.isMember("diff"))
		DLOG(ERROR)<<"No diff found in material..."<<std::endl;
	diff = val["diff"].asDouble();
}

void Object::accept(const Json::Value& val) {
	name = val["name"].asString();
	if (!val.isMember("texture")) {
		DLOG(ERROR)<<"No texture info find"<<std::endl;
	}
	if (val["texture"]["type"] == "pure")
		texture = new PureColorTexture();
	else if (val["texture"]["type"] == "picture")
		texture = new PictureTexture();
	else
		DLOG(ERROR)<<"Invalid Texture Type..."<<std::endl;
	texture->accept(val["texture"]);
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
