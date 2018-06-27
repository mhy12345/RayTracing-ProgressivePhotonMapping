#include "progressive_photon_mapping.h"
#include <string>
#include "../core/object.h"
#include <iostream>
#include <algorithm>
using namespace std;

ViewPoint::ViewPoint(Vector pos,Vector N,Color color,double stgh,int x,int y) : C(pos),N(N),color(color),strength(stgh),x(x),y(y) {}

ProgressivePhotonMapping::ProgressivePhotonMapping() {
	view_root = NULL;
}

ProgressivePhotonMapping::~ProgressivePhotonMapping() {
}


void ProgressivePhotonMapping::accept(const Json::Value& val) {
	Render::accept(val);
	Scene::accept(val,rx,ry);
	max_depth = val["max_depth"].asInt();
	start_rows = val["start_rows"].asInt();
	start_cols = val["start_cols"].asInt();
	bazier_quality = val["bazier_quality"].asInt();
	total_round = val["total_round"].asInt();
	photon_num = val["photon_num"].asInt();
	total_brightness = val["total_brightness"].asDouble();
	round_decay = val["round_decay"].asDouble();
	initial_r = val["initial_r"].asDouble();
	max_jump = val["max_jump"].asInt();
	board = new Color[rx*ry];
	for (int i=0;i<rx*ry;i++)
		board[i] = Color(0,0,0);
}

void ProgressivePhotonMapping::run() {
	std::cout<<"Start SPPM algorithm..."<<std::endl;
	bg_pic = new Color[rx*ry];
	int* samples_count = new int[rx*ry];
	for (int i=0;i<rx*ry;i++)
		samples_count[i] = 0;

	double current_r = initial_r;
	double energy = 1.0 / log(total_round);
	double current_e = energy;

	for (int iter = 0; iter < total_round; iter++) {
		for (int i=0;i<rx*ry;i++)
			bg_pic[i] = Color(0,0,0);
		std::cout<<"Round <"<<iter<<"> ... "<<std::endl;
		std::cout<<"View Point generate..."<<std::endl;
		std::cout<<"Current R = "<<current_r<<std::endl;
		view_pts.clear();
#ifdef USE_OPENMP
#pragma omp parallel num_threads(8)
#pragma omp for
#endif
		for (int i=start_rows;i<rx;i++)
			for (int j=start_cols;j<ry;j++) {
				double _i = i + (rand()*1.0/RAND_MAX-.5);
				double _j = j + (rand()*1.0/RAND_MAX-.5);
				Vector rayO;
				Vector rayD;
				camera->getRay(_i,_j,rayO,rayD);
				RayTracing(rayO,rayD,Color(1,1,1),i,j,0,current_e,0);
				samples_count[i*ry+j] ++;
			}
		std::cout<<"Build KDTree..."<<std::endl;
		buildKDTree(view_root,view_pts);
		std::cout<<"Photon Scattering"<<std::endl;
		double brightness = 0;
		for (auto &lgt : lights) 
			brightness += lgt->getBrightness();
		for (auto &lgt : lights) {
			int lgt_emit_count = photon_num*lgt->getBrightness()/brightness;
#ifdef USE_OPENMP
#pragma omp for
#endif
			for (int i=0;i<lgt_emit_count;i++) {
				Vector rayO,rayD;
				lgt->randomlyEmit(rayO,rayD);
				photonTracing(rayO,rayD,lgt->getColor(lgt->getCenter()),0,initial_r,total_brightness/photon_num,0,0);
			}
		}
		std::cout<<"Release KDTree..."<<std::endl;
		releaseKDTree(view_root);
		for (int i=0;i<rx;i++)
			for (int j=0;j<ry;j++)
				board[i*ry+j] += bg_pic[i*ry+j]*(1.0/samples_count[i*ry+j]);
		current_r *= round_decay;
		//current_e += 4;
	}
}

void ProgressivePhotonMapping::photonTracing(const Vector& rayO, const Vector& rayD, const Color& rayC, int depth, double r, double lambda, double dist,int diff_count) {
	if (depth > max_jump)
		return ;
	Collision obj_coll;
	const Object* obj;
	obj = findCollidedObject(rayO,rayD,obj_coll);
	if (!obj)return;
	dist += obj_coll.dist;
	if (obj->getMaterial().refl > feps) {
		Vector resO,resD;
		obj_coll.reflection(resO,resD);
		photonTracing(resO,resD,rayC,depth+1,r,lambda*obj->getMaterial().refl,dist,diff_count);
	}
	if (obj->getMaterial().refr > feps) {
		Color acol;
		if (!obj_coll.face) 
			acol = (obj->getAbsorb().getColor(0,0)*-obj_coll.dist).exp();
		else
			acol = Color(1,1,1);
		Vector resO,resD;
		obj_coll.refraction(resO,resD);
		photonTracing(resO,resD,rayC*acol,depth+1,r,lambda*obj->getMaterial().refr,dist,diff_count);
	}
	if (obj->getMaterial().diff > feps) {
		vector<const ViewPoint*> vps;
		queryKDTree(view_root,vps,obj_coll.getSurfaceC(),r);
		for (auto&w : vps) {
			if ((w->N ^ rayD)<-feps) {
				Color res = rayC * w->color 
					* ((r-(w->C-obj_coll.getSurfaceC()).len())/r)
					* lambda 
					* (1.0/(diff_count+1))
					* w->strength;
					//* obj->getMaterial().diff;
				bg_pic[w->x * ry + w->y] += res;
			}
		}
		Vector resO,resD;
		obj_coll.diffusion(resO,resD);
		photonTracing(resO,resD,rayC*obj->getColor(obj_coll.C),depth+1,r,lambda*obj->getMaterial().diff,dist+obj_coll.dist,diff_count+1);
	}
}

void ProgressivePhotonMapping::RayTracing(const Vector& rayO,const Vector& rayD,Color rayC,int xx,int yy,int depth,double lambda,double dist) {
	if (lambda < 1e-6 || depth > max_depth)
		return ;
	Collision obj_coll,lgt_coll;
	const Object* obj = findCollidedObject(rayO,rayD,obj_coll);
	const Light* lgt = findCollidedLight(rayO,rayD,lgt_coll);
	if ((!obj && !lgt)) {
		bg_pic[xx*ry+yy] += bg_color*lambda;
	}else if ((!obj && lgt) || (obj && lgt && obj_coll.dist > lgt_coll.dist)) {
		bg_pic[xx*ry+yy] += lgt->getColor(lgt_coll.C)*lambda;
	}else {
		if (obj->getMaterial().refr > feps) {
			Color acol;
			if (!obj_coll.face) 
				acol = (obj->getAbsorb().getColor(0,0)*-obj_coll.dist).exp();
			else
				acol = Color(1,1,1);
			Vector resO,resD;
			obj_coll.refraction(resO,resD);
			RayTracing(resO,resD,rayC*acol,xx,yy,depth+1,lambda*obj->getMaterial().refr,dist+obj_coll.dist);
		}
		if (obj->getMaterial().refl > feps) {
			Vector resO,resD;
			obj_coll.reflection(resO,resD);
			RayTracing(resO,resD,rayC,xx,yy,depth+1,lambda*obj->getMaterial().refl,dist+obj_coll.dist);
		}
		if (obj->getMaterial().diff > feps) {
#pragma omp critical
			view_pts.push_back(
					ViewPoint(
						obj_coll.getSurfaceC(),
						obj_coll.N,
						rayC*obj->getColor(obj_coll.C),
						lambda*obj->getMaterial().diff,
						xx,yy
						)
					);
		}
	}
}

void ProgressivePhotonMapping::buildKDTree(KDTreeNode* &node, std::vector<ViewPoint> &lst, int l,int r,int dim) {
	if (l == -1 && r == -1)
		l = 0, r = lst.size();
	if (l >= r)
		return;
	int mid = (l+r)>>1;
	switch(dim) {
		case 0: nth_element(lst.begin()+l,lst.begin()+mid,lst.begin()+r,ViewPointComparer<0>());
		case 1: nth_element(lst.begin()+l,lst.begin()+mid,lst.begin()+r,ViewPointComparer<1>());
		case 2: nth_element(lst.begin()+l,lst.begin()+mid,lst.begin()+r,ViewPointComparer<2>());
	}
	node = new KDTreeNode();
	node->value = lst[mid];
	//std::cout<<node->value.C.description()<<std::endl;
	node->lch = node->rch = NULL;
	node->split_dim = dim;
	node->bd_max = node->value.C;
	node->bd_min = node->value.C;
	buildKDTree(node->lch,lst,l,mid,(dim+1)%3);
	if (node->lch) {
		node->bd_max = each_max(node->bd_max,node->lch->bd_max);
		node->bd_min = each_min(node->bd_min,node->lch->bd_min);
	}
	buildKDTree(node->rch,lst,mid+1,r,(dim+1)%3);
	if (node->rch) {
		node->bd_max = each_max(node->bd_max,node->rch->bd_max);
		node->bd_min = each_min(node->bd_min,node->rch->bd_min);
	}
}

void ProgressivePhotonMapping::queryKDTree(KDTreeNode* node, vector<const ViewPoint*> &result, const Vector& pos, double r) {
	double dx,dy,dz;
	if (pos.getX() <= node->bd_max.getX() && pos.getX() >= node->bd_min.getX())
		dx = 0;
	else
		dx = min(abs(pos.getX()-node->bd_max.getX()),abs(pos.getX()-node->bd_min.getX()));
	if (pos.getY() <= node->bd_max.getY() && pos.getY() >= node->bd_min.getY())
		dy = 0;
	else
		dy = min(abs(pos.getY()-node->bd_max.getY()),abs(pos.getY()-node->bd_min.getY()));
	if (pos.getZ() <= node->bd_max.getZ() && pos.getZ() >= node->bd_min.getZ())
		dz = 0;
	else
		dz = min(abs(pos.getZ()-node->bd_max.getZ()),abs(pos.getZ()-node->bd_min.getZ()));

	if (dx*dx + dy*dy + dz*dz >r*r)
		return ;

	if ((node->value.C-pos).len()<=r)
		result.push_back(&(node->value));
	if (node->lch)queryKDTree(node->lch,result,pos,r);
	if (node->rch)queryKDTree(node->rch,result,pos,r);
}

void ProgressivePhotonMapping::releaseKDTree(KDTreeNode* &node) {
	if (!node)return;
	releaseKDTree(node->lch);
	releaseKDTree(node->rch);
	delete node;
}
