#include "camera.h"
#include "glog/logging.h"

Camera::Camera() {
}
Camera::~Camera() {
}

void Camera :: accept(const Json::Value& val) {
	position.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
	rx = val["rx"].asInt();
	ry = val["ry"].asInt();
	origin.accept(val["origin"]);
	LOG(INFO)<<"Camera : Data accpeted."<<std::endl;
}

Ray Camera::getRay(double scanX,double scanY) {
	return Ray(origin,position + scanX *dx + scanY*dy);
}
