#include "render.h"
#include "glog/logging.h"
#include "../display/paintboard.h"
#include <iostream>

Render::Render(){
}

Render::~Render() {
}

void Render::accept(const Json::Value& val) {
	rx = val["rx"].asInt();
	ry = val["ry"].asInt();
	std::cout<<"Reander created <"<<rx<<","<<ry<<">"<<std::endl;
}

void Render::update() {
	this->paint_board->update();
}

void Render::registerPaintBoard(PaintBoard* pb) {
	paint_board = pb;
	paint_board->init(rx,ry,&board);
}
