#include "raytracing.h"
#include <string>
#include "../core/object.h"
#include "../objects/sphere.h"
#include "../objects/area_light.h"
#include "../objects/plane.h"
#include "glog/logging.h"
#include <iostream>

RayTracing::RayTracing() {
	camera = NULL;
	bg_color = NULL;
}
RayTracing::~RayTracing() {
	if (camera)
		delete camera;
	if (bg_color)
		delete bg_color;
}

void RayTracing::accept(const Json::Value& val) {
	LOG(INFO)<<"RayTracing : "<<val.toStyledString()<<std::endl;
	LOG(INFO)<<"RayTracing : Init camera"<<std::endl;

	if (val["camera"]["type"].asString() == "default") {
		camera = new Camera();
		camera->accept(val["camera"]);
	}

	LOG(INFO)<<"RayTracing : Init objects"<<std::endl;
	for (int i=0;i<val["objects"].size();i++) {
		Json::Value v = val["objects"][i];
		std::string tag = v["type"].asString();
		LOG(INFO)<<"RayTracing : "<<tag<<"<"<<v["name"]<<">"<<std::endl;
		if (tag == "sphere") {
			objects.push_back(new Sphere());
			objects.back()->accept(v);
		}else if (tag == "plane") {
			objects.push_back(new Plane());
			objects.back()->accept(v);
		}else if(tag[0] == '#') {
		}else {
			LOG(ERROR)<<"Strange Object <"<<tag<<std::endl;
		}
	}

	LOG(INFO)<<"RayTracing : Init lights"<<std::endl;
	for (int i=0;i<val["lights"].size();i++) {
		Json::Value v = val["lights"][i];
		std::string tag = v["type"].asString();

		if (tag == "area_light") {
			lights.push_back(new AreaLight());
			lights.back()->accept(v);
		}else if (tag[0] == '#') {

		}else {
			LOG(ERROR)<<"Strange Object <"<<tag<<">"<<std::endl;
		}
	}
	bg_color = new Color();
	bg_color->accept(val["bg_color"]);
	max_depth = val["max_depth"].asInt();
	shade_quality = val["shade_quality"].asInt();
	spec_power = val["spec_power"].asInt();
	LOG(INFO)<<"RayTracing : Data accepted"<<std::endl;
	board = new Color*[camera->getRx()*camera->getRy()];
}


Color RayTracing::calcReflection(const Object& obj, int depth, unsigned& hash) {
	LOG(INFO)<<"calcReflection... <d="<<depth<<",h="<<hash<<">"<<std::endl;
	LOG(INFO)<<"REFLECTION POSITION : "<<obj.getCollision().description()<<std::endl;
	if (depth > max_depth) 
		return *bg_color;
	return rayTrace(obj.getCollision().C,obj.getCollision().D,depth,hash) * obj.getMaterial().refl;
}

Color RayTracing::calcDiffusion(const Object& obj) {
	LOG(INFO)<<"calcDiffusion..."<<std::endl;
	LOG(INFO)<<"DIFFUSION POSITION : "<<obj.getCollision().description()<<std::endl;
	Color color = obj.getColor();
	Color ret = color * *bg_color;
	for (auto &lgt : lights) {
		double shade = lgt->getShade(obj.getCollision().C,objects,shade_quality);
		ret += color * lgt->getColor() * shade * obj.getMaterial().diff;
		double spec_ratio = (obj.getCollision().C-lgt->getCenter()).unit() ^ obj.getCollision().N.unit();
		ret += color * lgt->getColor() * shade * pow(spec_ratio,spec_power) * obj.getMaterial().spec;
	}
	return ret;
}

const Object* RayTracing::findCollidedObject(const Vector& _rayO,const Vector& _rayD) {
	Vector rayO = _rayO;
	Vector rayD = _rayD;
	Object* ret = NULL;
	double cdist = inf;
	for (auto obj : objects) {
		if (obj->collideWith(rayO,rayD)) {
			if (obj->getCollision().dist < cdist) {
				cdist = obj->getCollision().dist;
				ret = obj;
			}
		}
	}
	return ret;
}

const Light* RayTracing::findCollidedLight(const Vector& _rayO, const Vector& _rayD) {
	Vector rayO = _rayO;
	Vector rayD = _rayD;
	Light* ret = NULL;
	double cdist = inf;
	for (auto &lgt : lights) {
		if (lgt->collideWith(rayO,rayD)) {
			if (lgt->getCollision().dist < cdist) {
				cdist = lgt->getCollision().dist;
				ret = lgt;
			}
		}
	}
	return ret;
}

Color RayTracing::rayTrace(const Vector& rayO, const Vector& rayD, int depth, unsigned& hash) {
	LOG(INFO)<<"Ray Trace... <d="<<depth<<",h="<<hash<<">"<<std::endl;
	const Object* obj = findCollidedObject(rayO,rayD);
	const Light* lgt = findCollidedLight(rayO,rayD);
	if (!obj && !lgt) {
		return *bg_color;
	}else if (!obj || (obj && lgt && obj->getCollision().dist > lgt->getCollision().dist)) {
		return lgt->getColor();
	}else {
		Color ret;
		if (obj->getMaterial().refl>feps)
			ret += calcReflection(*obj,depth+1,hash);
		if (obj->getMaterial().diff>feps || obj->getMaterial().spec>feps)
			ret += calcDiffusion(*obj);
		return ret;
	}
}

void RayTracing::run() {
	for (int i=0;i<camera->getRx();i++){
		printf("Render row #%d\n",i);
		for (int j=0;j<camera->getRy();j++) {
			LOG(INFO)<<"RENDER POSITION <"<<i<<","<<j<<">"<<std::endl;
			Vector rayO,rayD;
			camera->getRay(i,j,rayO,rayD);
			LOG(INFO)<<"RayO = "<<rayO.description()<<std::endl;
			LOG(INFO)<<"RayD = "<<rayD.description()<<std::endl;
			unsigned hash = 0;
			Color cc = rayTrace(rayO,rayD,0,hash);
			board[i*camera->getRy()+j] = new Color(cc);
			LOG(INFO)<<"Color = "<<cc.description()<<std::endl;
		}
	}

}

void RayTracing::update() {
	this->paint_board->update();
}

void RayTracing::registerPaintBoard(PaintBoard* pb) {
	assert(camera);
	paint_board = pb;
	paint_board->init(camera->getRx(),camera->getRy(),&board);
}
