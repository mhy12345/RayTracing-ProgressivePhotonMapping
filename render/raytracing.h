#ifndef RAYTRACING_H
#define RAYTRACING_H

#include "../core/gobject.h"
#include "../core/sceneloader.h"

class RayTracing : GObject{
	private:
	public:
		virtual void accept(SceneLoader* sl);
};

#endif
