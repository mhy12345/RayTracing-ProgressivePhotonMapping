#include "object.h"
#include <random>
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
	double cosI = N.reverse()^I;
	double n = face ? 1.0/belongs->getMaterial().refr_k : belongs->getMaterial().refr_k;
	double cosT2 = 1 - (n*n)*(1-cosI*cosI);
	if (cosT2 > feps) {
		resO = getBackfaceC();
		resD = I*n + N*(n*cosI - sqrt(cosT2));
	}else {
		reflection(resO,resD);
	}
}

void Collision::reflection(Vector& resO, Vector& resD)const {
	Vector _N = N*(N^I.reverse());
	resD = (_N*2-I.reverse());
	resO = getSurfaceC();
}

Vector Collision::getSurfaceC()const {
	return C+N*(feps*2);
}
Vector Collision::getBackfaceC()const {
	return C-N*(feps*2);
}

void Material::accept(const Json::Value& val) {
	if (!val.isMember("refl"))
		std::cout<<"No refl found in material..."<<std::endl;
	refl = val["refl"].asDouble();
	if (!val.isMember("spec"))
		std::cout<<"No spec found in material..."<<std::endl;
	spec = val["spec"].asDouble();
	if (!val.isMember("diff"))
		std::cout<<"No diff found in material..."<<std::endl;
	diff = val["diff"].asDouble();
	if (!val.isMember("refr"))
		std::cout<<"No refr found in material..."<<std::endl;
	refr = val["refr"].asDouble();
	if (!val.isMember("refr_k"))
		std::cout<<"No refr_k found in material..."<<std::endl;
	refr_k = val["refr_k"].asDouble();
}

void Object::accept(const Json::Value& val) {
	name = val["name"].asString();
	if (!val.isMember("texture")) {
		std::cout<<"No texture info find"<<std::endl;
	}
	if (val["texture"]["type"] == "pure")
		texture = new PureColorTexture();
	else if (val["texture"]["type"] == "picture")
		texture = new PictureTexture();
	else
		std::cout<<"Invalid Texture Type..."<<std::endl;
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
		"I = " + I.description() + "\n" + 
		"dist = " + std::string(buf) + "\n" +
		"face = " + (face?"true":"false");
}

const Texture& Object::getAbsorb()const {
	if (!absorb) {
		std::cout<<"The absorb not defined yet."<<std::endl;
	}
	return *absorb;
}
