#include "bazier_curve.h"
#include <cmath>
#include <cassert>
#include <iostream>

BazierCurve::BazierCurve(int id) : id(id) {
}

void BazierCurve::accept(const Json::Value& val) {
	Object::accept(val);
	Vector _Q;
	_Q.accept(val["position"]);
	Q = _Q.eigen();
	for (int i=0;i<4;i++) {
		px[i] = val["ctrl_pts"][id][0][i].asDouble();
		py[i] = val["ctrl_pts"][id][1][i].asDouble();
	}
	std::cout<<id<<" FINISH"<<std::endl;
	maxX = -1e100;
	minX = 1e100;
	maxR = -1e100;
	minR = 1e100;
	for (int i=0;i<4;i++) {
		maxX = std::max(maxX,py[i]);
		minX = std::min(minX,py[i]);
		maxR = std::max(maxR,px[i]);
		minR = std::min(minR,px[i]);
	}
}

Color BazierCurve::getColor(const Vector&)const {
	if (texture->getType() != TEXTURE_PURE_COLOR)
		std::cout<<"The getColor of BazierCurve only support PURE_COLOR_MODE"<<std::endl;
	return texture->getColor();
}
int BazierCurve::cylinderCollision(const Vector& _rayO,const Vector& rayD,double r) {
	Vector rayO = _rayO - Q;
	double ox,oy,dx,dy;
	ox = rayO.getY();
	oy = rayO.getZ();
	dx = rayD.getY();
	dy = rayD.getZ();
	double a,b,c;
	a = dx*dx+dy*dy;
	b = 2*(dx*ox+oy*dy);
	c = ox*ox+oy*oy - r*r;
	double delta = b*b-4*a*c;
	if (delta<=0)
		return 0;
	double k1 = (-b-sqrt(delta))/(2*a);
	double k2 = (-b+sqrt(delta))/(2*a);
	double x1 = rayO.getX() + k1 * rayD.getX();
	double x2 = rayO.getX() + k2 * rayD.getX();
	if (x1>x2)std::swap(x1,x2);
	if (x2<minX || x1>maxX) {
		return 0;
	}
	if (x1<minX && x2>maxX)
		return 2;//竖直射入
	if (x1>minX && x2<maxX)
		return 1;//水平射入
	return 3;
}
bool BazierCurve::checkCollision(const Vector& rayO,const Vector& rayD) {
	int max_s = cylinderCollision(rayO,rayD,maxR);
	int min_s = cylinderCollision(rayO,rayD,minR);
	if (max_s == 0 || min_s == 2)
		return false;
	return true;
}
bool BazierCurve::initArgs(double &t,double &u,double &theta,Vector rayO,Vector rayD,double _u = -1) {
	if (_u>=0)
		u = _u;
	else
		u = rand()*1.0/RAND_MAX;
	/*
	   rayD = rayD + rand()*1.0/RAND_MAX * 1e-6 * Vector(0,-1,1);
	   rayD = rayD + rand()*1.0/RAND_MAX * 1e-6 * Vector(-1,1,0);
	   rayD = rayD + rand()*1.0/RAND_MAX * 1e-6 * Vector(1,0,-1);
	   */
	double h = rand()*1.0/RAND_MAX *(maxX-minX)+maxX;
	t = ((Q.x() + h) - rayO.getX()) / rayD.getX();
	Vector C = rayO + t*rayD;
	Vector V = (Q+Vector(h,0,0) - C);
	assert(abs(V.getX())<feps);
	theta = rand()*1.0/RAND_MAX*M_PI*2;
	//theta = -atan2(V.getZ(),V.getY());
	if (V.getY()*V.getY() + V.getZ()*V.getZ() > maxR*maxR)
		return true;
	return true;
}

bool BazierCurve::collideWith(const Vector& rayO,const Vector& rayD,Collision& collision) {
	Eigen::Vector3d O = rayO.eigen();
	Eigen::Vector3d D = rayD.eigen();
	collision.dist = 1e100;
	if (!checkCollision(rayO,rayD))
		return false;
	for (int cnt=0;cnt<35 ;cnt++){
		double lr = .7;
		double t;
		double u;
		double theta;
		if (!initArgs(t,u,theta,rayO,rayD,cnt<2?cnt:-1)) {
			continue;
		}
		Eigen::Vector3d args(t,u,theta);
		bool flag = false;
		for (int iter=0;iter<20;iter++) {
			t = args.x();
			u = args.y();
			theta = args.z();
			if (u<-.5 || u>1.5)break;
			Eigen::Vector3d F = getF(t,u,theta,O,D);
			Eigen::Matrix3d dF = getdF(t,u,theta,O,D);
			/*
			   std::cout<<"args = \n"<<args<<std::endl;
			   std::cout<<"F = \n"<<F<<std::endl;
			   std::cout<<"dF = \n"<<dF<<std::endl;
			   std::cout<<"S = \n"<<getS(u,theta)<<std::endl;
			   std::cout<<"C = \n"<<getC(t,O,D)<<std::endl;
			   */

			if (std::max(std::max(std::abs(F.x()),std::abs(F.y())),std::abs(F.z())) < 1e-7) {
				flag = true;
				break;
			}
			args = args - (dF.inverse()*F)*lr;
		}
		if (!flag)continue;
		//std::cout<<"t,u,theta = "<<t<<" "<<u<<" "<<theta<<std::endl;
		if (t<0)continue;
		if (u<0 || u>1)continue;
		if (t > collision.dist)continue;
		collision.C = rayO + t*rayD;
		collision.dist = t;
		Eigen::Vector3d pspu(getdP(py,u), sin(theta)*getdP(px,u),cos(theta)*getdP(px,u));
		Eigen::Vector3d pspt(0, cos(theta)*getP(px,u) , -sin(theta)*getP(px,u));
		Eigen::Vector3d _N = pspu.cross(pspt);
		collision.N = Vector(_N).unit();
		collision.face = (collision.N ^ rayD.reverse()) > 0;
		if (!collision.face)
			collision.N = collision.N.reverse();

		collision.I = rayD;
		//collision.D = 2*collision.N*(collision.N^rayD.reverse()) - rayD.reverse();
		collision.belongs = this;
		/*
		   std::cout<<"I = "<<rayD.description()<<std::endl;
		   std::cout<<"N = "<<collision.N.description()<<std::endl;
		   std::cout<<"D = "<<collision.D.description()<<std::endl;*/
	}
	if (collision.dist < 1e90)
	{
		/*
		   std::cout<<"I = "<<rayD.description()<<std::endl;
		   std::cout<<"N = "<<collision.N.description()<<std::endl;
		   std::cout<<"_N= "<<(collision.N*(collision.N^rayD.reverse())).description()<<std::endl;
		   std::cout<<"D = "<<collision.D.description()<<std::endl;
		   std::cout<<name<<" "<<collision.description()<<std::endl;
		   */
		return true;
	}
	return false;
}

inline double BazierCurve::getP(double* p,double t) {
	return 1*p[0]*(1-t)*(1-t)*(1-t) +
		3*p[1]*t*(1-t)*(1-t) +
		3*p[2]*t*t*(1-t) +
		1*p[3]*t*t*t;
}

inline double BazierCurve::getdP(double* p,double t) {
	return -3*p[0]*(1-t)*(1-t) +
		3*p[1]*(1-t)*(1-t) +
		-6*p[1]*t*(1-t) +
		6*p[2]*(1-t)*t +
		-3*p[2]*t*t +
		3*p[3]*t*t;
}

Eigen::Vector3d BazierCurve::getC(double t,const Eigen::Vector3d& O,const Eigen::Vector3d& D) {
	return O + D*t;
}

Eigen::Vector3d BazierCurve::getS(double u,double theta) {
	return Q + Eigen::Vector3d( getP(py,u), sin(theta)*getP(px,u), cos(theta)*getP(px,u));
}

Eigen::Vector3d BazierCurve::getF(double t,double u,double theta, const Eigen::Vector3d& O,const Eigen::Vector3d& D) {
	return getC(t,O,D) - getS(u,theta);
}

Eigen::Matrix3d BazierCurve::getdF(double t,double u,double theta, const Eigen::Vector3d& O,const Eigen::Vector3d& D) {
	Eigen::Matrix3d res;
	res << 
		D.x() , -getdP(py,u) , 0,
		D.y() , -sin(theta)*getdP(px,u) , -cos(theta)*getP(px,u) ,
		D.z() , -cos(theta)*getdP(px,u) , +sin(theta)*getP(px,u) ;
	return res;
}
