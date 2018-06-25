#ifndef RENDER_H
#define RENDER_H
#include "json/json.h"
#include "../display/paint_board.h"
#include "../core/vector.h"
#include "../core/light.h"
#include "../core/object.h"
#include "../core/color.h"
#include "../core/camera.h"

class Render {
	protected:
		PaintBoard* paint_board;
		Color *board;
		int rx,ry;
	public:
		Render();
		~Render();
		virtual void accept(const Json::Value &val) = 0;
		virtual void run() = 0;
		void registerPaintBoard(PaintBoard* pb) ;
		void update();
};

#endif
