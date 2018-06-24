#include "object.h"
#include <random>
#include <glog/logging.h>
#include <cstdlib>
#include <iostream>

Object::Object() {
	hash = rand();
}

Object::~Object() {
	if (texture) 
		delete texture;
	if (absorb)
		delete absorb;
}

void Collision::refraction(Vector& resO, Vector& resD)const {
	LOG(INFO)<<"face = "<<face<<std::endl;
	Vector _N = N*(N^D);
	Vector In = (_N*2 - D).reverse();
	double cosI = N.reverse()^In;
	double n = face ? 1.0/belongs->getMaterial().refr_k : belongs->getMaterial().refr_k;
	double cosT2 = 1 - (n*n)*(1-cosI*cosI);
	if (cosT2 > feps) {
		resO = getBackfaceC();
		resD = In*n + N*(n*cosI - sqrt(cosT2));
		DLOG(INFO)<<"The refrection..."<<std::endl;
		DLOG(INFO)<<"In = "<<In.description()<<std::endl;
		DLOG(INFO)<<"Out = "<<resD.description()<<std::endl;
	}else {
		resO = getSurfaceC();
		resD = D;
		DLOG(INFO)<<"The full refraction..."<<std::endl;
		DLOG(INFO)<<"Out = "<<resD.description()<<std::endl;
	}
}

Vector Collision::getSurfaceC()const {
	return C+N*(feps*2);
}
Vector Collision::getBackfaceC()const {
	return C-N*(feps*2);
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
	if (!val.isMember("refr"))
		DLOG(ERROR)<<"No refr found in material..."<<std::endl;
	refr = val["refr"].asDouble();
	if (!val.isMember("refr_k"))
		DLOG(ERROR)<<"No refr_k found in material..."<<std::endl;
	refr_k = val["refr_k"].asDouble();
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
		DLOG(FATAL)<<"Invalid Texture Type..."<<std::endl;
	texture->accept(val["texture"]);
	material.accept(val["material"]);
	if (material.refr>feps)
	{
		absorb = new PureColorTexture();
		absorb->accept(val["absorb"]);
	}
}

std::string Collision::description()const {
	static char buf[20];
	sprintf(buf,"%f",dist);
	return std::string("Collision : \n") +
		"C = "+C.description() +"\n" +
		"N = " + N.description() + "\n" +
		"D = " + D.description() + "\n" + 
		"dist = " + std::string(buf) + "\n" +
		"face = " + (face?"true":"false");
}

const Texture& Object::getAbsorb()const {
	if (!absorb) {
		LOG(FATAL)<<"The absorb not defined yet."<<std::endl;
	}
	return *absorb;
}
