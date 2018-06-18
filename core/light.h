#ifndef LIGHT_H
#define LIGHT_H

#include "gobject.h"
#include "sceneloader.h"
#include "vector.h"
#include "color.h"

class Light : GObject {
	private:
		Vector position; 
		Color color;
	public:
		virtual void accept(SceneLoader sl);
		const Vector& getPosition() {return position;}
};

#endif
