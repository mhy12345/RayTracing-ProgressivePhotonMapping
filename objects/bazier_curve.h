#ifndef BAZIER_CURVE_H
#define BAZIER_CURVE_H

#include "../core/vector.h"
#include "../core/object.h"
#include "Eigen/Dense"

class BazierCurve : public Object {
	private:
		Eigen::Vector3d Q;
		//Eigen::Vector3d W;
		Color color;
		int id;
		double px[4];
		double py[4];
		double maxX,minX;
		double maxR;
	public:
		BazierCurve(int id);
		virtual void accept(const Json::Value& val);
		Color getColor(const Vector&)const;
		bool collideWith(const Vector& rayO,const Vector& rayD,Collision& collision);
		double getP(double* p,double t);
		double getdP(double* p,double t);
		Eigen::Vector3d getC(double t,const Eigen::Vector3d& O,const Eigen::Vector3d& D);
		Eigen::Vector3d getdC(double t);
		Eigen::Vector3d getS(double u,double theta);
		Eigen::Vector3d getF(double t,double u,double theta,const Eigen::Vector3d& O,const Eigen::Vector3d& D);
		Eigen::Matrix3d getdF(double t,double u,double theta,const Eigen::Vector3d& O,const Eigen::Vector3d& D);
		bool initArgs(double &t,double &u,double &theta,Vector rayO,Vector rayD,double _u);
		bool checkCollision(const Vector& rayO,const Vector& rayD);
};

#endif
