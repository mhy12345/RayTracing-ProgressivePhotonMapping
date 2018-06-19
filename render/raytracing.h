#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <vector>
#include "../core/light.h"
#include "../core/camera.h"
#include "../core/object.h"
#include "../core/color.h"
#include "json/json.h"

class RayTracing{
	private:
		Camera *camera;
		std::vector<Light*> lights;
		std::vector<Object*> objects;
		Color *bg_color;
		Color **board;
	public:
		RayTracing();
		~RayTracing();
		virtual void accept(const Json::Value& val);
		const Object* findCollidedObject(const Ray &ray);
		const Light* findCollidedLight(const Ray& ray);
		Color rayTrace(const Ray& ray,unsigned& hash);
		void run();
};

#endif
