#include "progressive_photon_mapping.h"
#include <string>
#include "../core/object.h"
#include "../objects/sphere.h"
#include "../objects/area_light.h"
#include "../objects/plane.h"
#include "../objects/bazier_curve.h"
#include <iostream>
using namespace std;

ProgressivePhotonMapping::ProgressivePhotonMapping() {
}

ProgressivePhotonMapping::~ProgressivePhotonMapping() {
}


void ProgressivePhotonMapping::accept(const Json::Value& val) {
	Render::accept(val);
	Scene::accept(val,rx,ry);
	max_depth = val["max_depth"].asInt();
	start_rows = val["start_rows"].asInt();
	bazier_quality = val["bazier_quality"].asInt();
	total_round = val["total_round"].asInt();
	board = new Color[rx*ry];
	for (int i=0;i<rx*ry;i++)
		board[i] = Color(1,1,1);
}

void ProgressivePhotonMapping::run() {
	for (int iter = 0; iter < total_round; iter++) {
		for (int i=0;i<rx;i++)
			for (int j=0;j<ry;j++) {
				double _i = i + (rand()*1.0/RAND_MAX-.5);
				double _j = j + (rand()*1.0/RAND_MAX-.5);
				Vector rayO;
				Vector rayD;
				camera->getRay(_i,_j,rayO,rayD);
				RayTracing(rayO,rayD,0,1,0);
			}
	}
}

void ProgressivePhotonMapping::RayTracing(const Vector& rayO,const Vector& rayD,int depth,double lambda,double dist) {
	Collision obj_coll,lgt_coll;
	Object* obj = findCollidedObject(rayO,rayD,obj_coll);
	Light* lgt = findCollidedObject(rayO,rayD,lgt_coll);
	if (obj->getMaterials().refr > feps) {
		Vector resO,resD;
		obj_coll.refraction(resO,resD);
		RayTracing(resO,resD,depth+1,lambda*obj->getMaterials().refr,dist+obj_coll.dist);
	}
	if (obj->getMaterials().refl > feps) {
		Vector resO,resD;
		obj_coll.reflection(resO,resD);
		RayTracing(resO,resD,depth+1,lambda*obj->getMaterials().refr,dist+obj_coll.dist);
	}
	if (obj->getMaterials().diff > feps) {
		view_pts.push_back(ViewPoint(obj_coll.getSurfaceC(),lambda*obj->getMaterials().diff));
	}
}
