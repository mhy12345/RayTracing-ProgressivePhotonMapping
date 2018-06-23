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

void Collision::refraction(Vector& resO, Vector& resD)const {
	assert(N.isUnit());
	assert(D.isUnit());
	assert(belongs);
	double sin_theta = (N*D).len();
	double sin_theta_;
	if (face)
		sin_theta_ = sin_theta / belongs->getMaterial().refr_k;
	else
		sin_theta_ = sin_theta * belongs->getMaterial().refr_k;
	if (sin_theta_>=1-feps) {
		resD = D;
		resO = getSurfaceC();
		DLOG(INFO)<<"The full refraction..."<<std::endl;
		DLOG(INFO)<<"	resD..."<<resD.description()<<std::endl;
		DLOG(INFO)<<"	resO..."<<resO.description()<<std::endl;
		DLOG(INFO)<<"	IN_ANGLE_SIN..."<<sin_theta<<std::endl;
		DLOG(INFO)<<"	OU_ANGLE_SIN..."<<sin_theta_<<std::endl;
		return ;
	}
	Vector _N = N*(N^D);
	Vector In = (_N*2 - D).reverse();
//if (!In.isUnit()) std::cout<<"FUCk1"<<std::endl;
	double angle;
	double in_angle = asin(sin_theta);
	double out_angle = asin(sin_theta_);
	LOG(INFO)<<"IN_ANGLE = "<<asin(sin_theta)<<std::endl;
	LOG(INFO)<<"OU_ANGLE = "<<asin(sin_theta_)<<std::endl;
	angle = out_angle - in_angle;

	LOG(INFO)<<"The refrection..."<<std::endl;
	LOG(INFO)<<"In = "<<In.description()<<std::endl;
	Eigen::AngleAxisd V1(-angle, (N*D).unit().eigen());
	In = Vector(V1.matrix() * In.eigen());
	assert(In.isUnit());
	//if (!In.isUnit()) std::cout<<"FUCk2"<<In.len()<<" "<<In.description()<<std::endl;
	resD = Vector(In);
	resO = getBackfaceC();
	LOG(INFO)<<"N = "<<N.description()<<std::endl;
	LOG(INFO)<<"Out = "<<In.description()<<std::endl;
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
		"D = " + D.description() + "\n" + 
		"dist = " + std::string(buf) + "\n" +
		"face = " + (face?"true":"false");
}
