#include "camera.h"
#include "glog/logging.h"

Camera::Camera() {
}
Camera::~Camera() {
}

void Camera :: accept(const Json::Value& val) {
	DLOG(INFO)<<"Camera : Init position"<<std::endl;
	position.accept(val["position"]);
	DLOG(INFO)<<"Camera : Init dx/dy"<<std::endl;
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
	DLOG(INFO)<<"Camera : Init rx/ry"<<std::endl;
	rx = val["rx"].asInt();
	ry = val["ry"].asInt();
	DLOG(INFO)<<"Camera : Init origin"<<std::endl;
	origin.accept(val["origin"]);
	DLOG(INFO)<<"Camera : Data accpeted."<<std::endl;
	DLOG(INFO)<<"Camera : rx/ry = "<<rx<<" "<<ry<<std::endl;
	assert(rx>1 && ry>1);
}

void Camera::getRay(double scanX,double scanY, Vector& rayO, Vector& rayD) {
	rayO = origin;
	rayD = ((position + scanX*dx/(rx-1) + scanY*dy/(ry-1))-origin).unit();
}
