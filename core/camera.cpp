#include "camera.h"

Camera::Camera(int rx,int ry):rx(rx),ry(ry) {
}
Camera::~Camera() {
}

void Camera :: accept(const Json::Value& val) {
	position.accept(val["position"]);
	dx.accept(val["dx"]);
	dy.accept(val["dy"]);
	origin.accept(val["origin"]);
}

void Camera::getRay(double scanX,double scanY, Vector& rayO, Vector& rayD) {
	rayO = origin;
	rayD = ((position + scanX*dx/(rx-1) + scanY*dy/(ry-1))-origin).unit();
}
