#include "raytracing.h"
#include <string>
#include "../core/object.h"
#include "../objects/sphere.h"
#include "../objects/area_light.h"
#include "../objects/plane.h"
#include "glog/logging.h"
#include <iostream>

#ifdef USE_OPENMP
#include <omp.h>
#pragma message("The openmp is enable!")
#endif

RayTracing::RayTracing() {
	camera = NULL;
}

RayTracing::~RayTracing() {
	if (camera)
		delete camera;
}

void RayTracing::accept(const Json::Value& val) {
	DLOG(INFO)<<"RayTracing : "<<val.toStyledString()<<std::endl;
	DLOG(INFO)<<"RayTracing : Init camera"<<std::endl;

	if (val["camera"]["type"].asString() == "default") {
		camera = new Camera();
		camera->accept(val["camera"]);
	}

	DLOG(INFO)<<"RayTracing : Init objects"<<std::endl;
	for (int i=0;i<val["objects"].size();i++) {
		Json::Value v = val["objects"][i];
		std::string tag = v["type"].asString();
		DLOG(INFO)<<"RayTracing : "<<tag<<"<"<<v["name"]<<">"<<std::endl;
		if (tag == "sphere") {
			objects.push_back(new Sphere());
			objects.back()->accept(v);
		}else if (tag == "plane") {
			objects.push_back(new Plane());
			objects.back()->accept(v);
		}else if(tag[0] == '#') {
		}else {
			DLOG(ERROR)<<"Strange Object <"<<tag<<std::endl;
		}
	}

	DLOG(INFO)<<"RayTracing : Init lights"<<std::endl;
	for (int i=0;i<val["lights"].size();i++) {
		Json::Value v = val["lights"][i];
		std::string tag = v["type"].asString();

		if (tag == "area_light") {
			lights.push_back(new AreaLight());
			lights.back()->accept(v);
		}else if (tag[0] == '#') {

		}else {
			DLOG(ERROR)<<"Strange Object <"<<tag<<">"<<std::endl;
		}
	}
	bg_color.accept(val["bg_color"]);
	max_depth = val["max_depth"].asInt();
	shade_quality = val["shade_quality"].asInt();
	spec_power = val["spec_power"].asInt();
	start_rows = val["start_rows"].asInt();
	DLOG(INFO)<<"RayTracing : Data accepted"<<std::endl;
	board = new Color[camera->getRx()*camera->getRy()];
	for (int i=0;i<camera->getRx()*camera->getRy();i++)
		board[i] = Color(1,1,1);
}


Color RayTracing::calcReflection(const Object& obj, const Collision& obj_coll, int depth, unsigned&hash) {
	//LOG(INFO)<<"calcReflection... <d="<<depth<<",h="<<hash<<">"<<std::endl;
	DLOG(INFO)<<"REFLECTION POSITION : "<<obj_coll.description()<<std::endl;
	if (depth > max_depth) {
		hash = hash * 17 + 233;
		return bg_color;
	}
	hash = hash * 17 + obj.getHash();
	return (rayTrace(obj_coll.getSurfaceC(),obj_coll.D,depth,hash) * obj.getMaterial().refl).adjust();
}

Color RayTracing::calcRefraction(const Object& obj, const Collision& obj_coll, int depth, unsigned& hash) {
	LOG(INFO)<<"calcRefraction... <d="<<depth<<",h="<<hash<<">"<<std::endl;
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
	//LOG(INFO)<<"calcDiffusion..."<<std::endl;
	Color color = obj.getColor(obj_coll.C);
	DLOG(INFO)<<"DIFFUSION POSITION : "<<obj_coll.description()<<std::endl;
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

const Object* RayTracing::findCollidedObject(const Vector& _rayO,const Vector& _rayD,Collision& resColl) {
	Vector rayO = _rayO;
	Vector rayD = _rayD;
	Object* ret = NULL;
	resColl.dist = inf;
	for (auto obj : objects) {
		Collision obj_coll;
		if (obj->collideWith(rayO,rayD,obj_coll)) {
			if (obj_coll.dist < resColl.dist) {
				ret = obj;
				resColl = obj_coll;
			}
		}
	}
	return ret;
}

const Light* RayTracing::findCollidedLight(const Vector& _rayO, const Vector& _rayD,Collision& resColl) {
	Vector rayO = _rayO;
	Vector rayD = _rayD;
	Light* ret = NULL;
	resColl.dist = inf;
	for (auto &lgt : lights) {
		Collision lgt_coll;
		if (lgt->collideWith(rayO,rayD,lgt_coll)) {
			if (lgt_coll.dist < resColl.dist) {
				resColl = lgt_coll;
				ret = lgt;
			}
		}
	}
	return ret;
}

Color RayTracing::rayTrace(const Vector& rayO, const Vector& rayD, int depth, unsigned& hash) {
	//LOG(INFO)<<"Ray Trace... <d="<<depth<<",h="<<hash<<">"<<std::endl;
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
	LOG(INFO)<<"Sampling..."<<std::endl;
	hash_table = new unsigned*[camera->getRx()];
	for (int i=0;i<camera->getRx();i++)
		hash_table[i] = new unsigned[camera->getRy()];
#ifdef USE_OPENMP
#pragma omp parallel
#pragma omp for schedule(dynamic,5)
#endif
	for (int _i=start_rows;_i<camera->getRx();_i++){
		int i = _i;
#ifdef USE_OPENMP
		if (!i)
			std::cout<<omp_get_num_threads()<<std::endl;
#endif
		printf("Render row #%d\n",i);
		for (int j=0;j<camera->getRy();j++) {
			LOG(INFO)<<"RENDER POSITION <"<<i<<","<<j<<">"<<std::endl;
			Vector rayO,rayD;
			camera->getRay(i,j,rayO,rayD);
			DLOG(INFO)<<"	RayO = "<<rayO.description()<<std::endl;
			DLOG(INFO)<<"	RayD = "<<rayD.description()<<std::endl;
			Color cc = rayTrace(rayO,rayD,0,hash_table[i][j]);
			board[i*camera->getRy()+j] = cc;
			LOG(INFO)<<"Color = "<<cc.description()<<std::endl;
		}
	}
	LOG(INFO)<<"Resampling..."<<std::endl;
#ifdef USE_OPENMP
#pragma omp for schedule(dynamic,5)
#endif
	for (int i=0;i<camera->getRx();i++) {
		for (int j=0;j<camera->getRy();j++) {
			bool flag = false;
			if (i!=0 && hash_table[i][j] != hash_table[i-1][j])
				flag = true;
			if (i!=camera->getRx()-1 && hash_table[i][j] != hash_table[i+1][j])
				flag = true;
			if (j!=0 && hash_table[i][j] != hash_table[i][j-1])
				flag = true;
			if (j!=camera->getRy()-1 && hash_table[i][j] != hash_table[i][j+1])
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
				board[i*camera->getRy() + j] = res;
			}
		}
	}
	for (int i=0;i<camera->getRx();i++) {
		delete[] hash_table[i];
	}
	delete[] hash_table;
}

void RayTracing::update() {
	this->paint_board->update();
}

void RayTracing::registerPaintBoard(PaintBoard* pb) {
	assert(camera);
	paint_board = pb;
	paint_board->init(camera->getRx(),camera->getRy(),&board);
}
