#include "raytracing.h"
#include <string>
#include <iostream>

#ifdef USE_OPENMP
#include <omp.h>
#pragma message("The openmp is enable!")
#endif

RayTracing::RayTracing() {
}

RayTracing::~RayTracing() {
}

void RayTracing::accept(const Json::Value& val) {
	Render::accept(val);
	Scene::accept(val,rx,ry);

	max_depth = val["max_depth"].asInt();
	shade_quality = val["shade_quality"].asInt();
	spec_power = val["spec_power"].asInt();
	start_rows = val["start_rows"].asInt();
	bazier_quality = val["bazier_quality"].asInt();
	board = new Color[rx*ry];
	for (int i=0;i<rx*ry;i++)
		board[i] = Color(1,1,1);
}


Color RayTracing::calcReflection(const Object& obj, const Collision& obj_coll, int depth, unsigned&hash) {
	if (depth > max_depth) {
		hash = hash * 17 + 233;
		return bg_color;
	}
	hash = hash * 17 + obj.getHash();
	Vector resO,resD;
	obj_coll.reflection(resO,resD);
	return (rayTrace(resO,resD,depth,hash) * obj.getMaterial().refl).adjust();
}

Color RayTracing::calcRefraction(const Object& obj, const Collision& obj_coll, int depth, unsigned& hash) {
	if (depth > max_depth) {
		hash = hash * 19 + 233;
		return bg_color;
	}
	hash = hash * 19 + obj.getHash();
	Vector rayD;
	Vector rayO;
	Color acol;
	obj_coll.refraction(rayO,rayD);
	Color rcol = rayTrace(rayO,rayD,depth,hash);
	if (obj_coll.face)
		rcol = rcol*obj.getMaterial().refr;
	if (!obj_coll.face)
		acol = (obj.getAbsorb().getColor(0,0)*-obj_coll.dist).exp();
	else
		acol = Color(1,1,1);

	return (rcol*acol).adjust();
}

Color RayTracing::calcDiffusion(const Object& obj, const Collision& obj_coll) {
	Color color = obj.getColor(obj_coll.C);
	Color ret = color * bg_color *(1-obj.getMaterial().diff-obj.getMaterial().spec);
	for (auto &lgt : lights) {
		double shade = lgt->getShade(obj_coll.getSurfaceC(),objects,shade_quality);
		ret += color * lgt->getColor(lgt->getCenter()) * shade * obj.getMaterial().diff;
		double spec_ratio = (lgt->getCenter() - obj_coll.C).unit() ^ obj_coll.N.unit();
		if (spec_ratio > 0)
			ret += color * lgt->getColor(lgt->getCenter()) * shade * pow(spec_ratio,spec_power) * obj.getMaterial().spec;
	}
	return ret.adjust();
}

Color RayTracing::rayTrace(const Vector& rayO, const Vector& rayD, int depth, unsigned& hash) {
	Collision obj_coll,lgt_coll;
	const Object* obj = findCollidedObject(rayO,rayD,obj_coll);
	const Light* lgt = findCollidedLight(rayO,rayD,lgt_coll);
	if (!obj && !lgt) {
		hash = hash * 17 + 235;
		return bg_color;
	}else if (!obj || (obj && lgt && obj_coll.dist > lgt_coll.dist)) {
		hash = hash * 17 + lgt->getHash();
		return lgt->getColor(lgt->getCenter());
	}else {
		Color ret;
		if (obj->getMaterial().refl>feps)
			ret += calcReflection(*obj,obj_coll,depth+1,hash);
		if (obj->getMaterial().diff>feps || obj->getMaterial().spec>feps)
			ret += calcDiffusion(*obj,obj_coll);
		if (obj->getMaterial().refr>feps)
			ret += calcRefraction(*obj,obj_coll,depth+1,hash);
		return ret.adjust();
	}
}

void RayTracing::run() {
	hash_table = new unsigned*[rx];
	for (int i=0;i<rx;i++)
		hash_table[i] = new unsigned[ry];
#ifdef USE_OPENMP
#pragma omp parallel
#pragma omp for schedule(dynamic,3)
#endif
	for (int _i=start_rows;_i<rx;_i++){
		int i = _i;
#ifdef USE_OPENMP
		if (!i)
			std::cout<<omp_get_num_threads()<<std::endl;
#endif
		printf("Render row #%d\n",i);
		for (int j=0;j<ry;j++) {
			Vector rayO,rayD;
			camera->getRay(i,j,rayO,rayD);
			Color cc = rayTrace(rayO,rayD,0,hash_table[i][j]);
			board[i*ry+j] = cc;
		}
	}
#ifdef USE_OPENMP
#pragma omp for schedule(dynamic,5)
#endif
	for (int i=0;i<rx;i++) {
		printf("Resample row #%d\n",i);
		for (int j=0;j<ry;j++) {
			bool flag = false;
			if (i!=0 && hash_table[i][j] != hash_table[i-1][j])
				flag = true;
			if (i!=rx-1 && hash_table[i][j] != hash_table[i+1][j])
				flag = true;
			if (j!=0 && hash_table[i][j] != hash_table[i][j-1])
				flag = true;
			if (j!=ry-1 && hash_table[i][j] != hash_table[i][j+1])
				flag = true;
			if (flag) {
				Color res;
				for (int k1=-1; k1<=1; k1++) {
					for (int k2=-1; k2<=1; k2++) {
						unsigned hash = 0;
						Vector rayO,rayD;
						camera->getRay(i+k1/3.0,j+k2/3.0,rayO,rayD);
						res += rayTrace(rayO,rayD,0,hash);
					}
				}
				res = res*(1.0/9);
				board[i*ry + j] = res;
			}
		}
	}
	for (int i=0;i<rx;i++) {
		delete[] hash_table[i];
	}
	delete[] hash_table;
}

