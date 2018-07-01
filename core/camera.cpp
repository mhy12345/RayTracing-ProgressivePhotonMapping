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
	fdepth = val["fdepth"].asDouble();
}

void Camera::getRay(double scanX,double scanY, Vector& rayO, Vector& rayD) {
	Vector _rayO,_rayD;
	_rayO = origin;
	_rayD = ((position + scanX*dx/(rx-1) + scanY*dy/(ry-1))-origin).unit();
	addDepth(_rayO,_rayD,rayO,rayD);
}

void Camera::addDepth(const Vector& rayO,const Vector& rayD,Vector& resO,Vector& resD) {
	Vector FocalPlanePoint = rayO + rayD*fdepth;
	resO =	rayO + Vector::randomVectorOnSphere()*fdepth;
	resD = (FocalPlanePoint - rayO).unit();
}
