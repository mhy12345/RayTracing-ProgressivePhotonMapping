#ifndef GOBJECT_H
#define GOBJECT_H

#include "sceneloader.h"
class SceneLoader;
class GObject {
	public:
		GObject();
		virtual ~GObject();
		virtual void accept(SceneLoader* sl) = 0;
};

#endif
