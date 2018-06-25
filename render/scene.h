#ifndef SCENE_H
#define SCENE_H
#include "../core/camera.h"
#include "../core/light.h"
#include "../core/object.h"

class Scene {
	protected:
		Camera *camera;
		std::vector<Light*> lights;
		std::vector<Object*> objects;
		Color bg_color;
	public:
		Scene();
		~Scene();
		void accept(const Json::Value& val,int _rx,int _ry);
		const Object* findCollidedObject(const Vector &rayO,const Vector& rayD,Collision& coll);
		const Light* findCollidedLight(const Vector& rayO,const Vector& rayD, Collision& coll);
};

#endif
