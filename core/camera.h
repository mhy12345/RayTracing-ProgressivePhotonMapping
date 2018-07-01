#ifndef CAMERA_H
#define CAMERA_H
#include "vector.h"

class Camera {
	private:
		Vector position;
		Vector dx,dy;
		Vector origin;
		int rx,ry;
		double fdepth;
	public:
		Camera(int rx,int ry);
		~Camera();
		void accept(const Json::Value& val);
		void getRay(double scanX, double scanY, Vector &rayO, Vector &rayD);
		void addDepth(const Vector& rayO,const Vector& rayD,Vector& resO,Vector& resD);
};

#endif
