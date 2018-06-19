#include "raytracing.h"
#include <string>
#include "../core/object.h"
#include "../objects/sphere.h"
#include "glog/logging.h"

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
	if (val["camera"]["type"].asString() == "default") {
		camera = new Camera();
		camera->accept(val["camera"]);
	}

	for (int i=0;i<val["objects"].size();i++) {
		Json::Value v = val["objects"][i];
		std::string tag = v["type"].asString();

		if (tag == "sphere") {
				objects.push_back(new Sphere());
				objects.back()->accept(v);
		}else {
			LOG(WARNING)<<"Strange Object..."<<std::endl;
		}
	}

	for (int i=0;i<val["lights"].size();i++) {
		Json::Value v = val["lights"][i];
		std::string tag = v["type"].asString();

		if (tag == "area_light") {
				lights.push_back(new AreaLight());
				lights.back()->accept(v);
		}else {
			LOG(WARNING)<<"Strange Object..."<<std::endl;
		}
	}
	bg_color = new Color();
	bg_color->accept(val["bg_color"]);
	LOG(INFO)<<"RayTracing : Data accepted"<<std::endl;
}

const Object* RayTracing::findCollidedObject(const Ray& ray) {
	Object* ret = NULL;
	double cdist = inf;
	for (auto obj : objects) {
		if (obj->collideWith(ray)) {
			if (obj->getCollision().dist < cdist) {
				cdist = obj->getCollision().dist;
				ret = obj;
			}
		}
	}
	return ret;
}

const Light* RayTracing::findCollidedLight(const Ray& ray) {
	Light* ret = NULL;
}

Color RayTracing::rayTrace(const Ray& ray,unsigned& hash) {
	const Object* obj = findCollidedObject(ray);
	const Light* lgt = findCollidedLight(ray);
	if (!obj) {
	}else {
	}
}

void RayTracing::run() {
	board = new Color*[camera->getRx()*camera->getRy()];
	for (int i=0;i<camera->getRx();i++)
		for (int j=0;j<camera->getRy();j++) {
			Ray ray = camera->getRay(i,j);
			unsigned hash = 0;
			Color cc = rayTrace(ray,hash);
			board[i*camera->getRy()+j] = new Color(cc);
		}

}
