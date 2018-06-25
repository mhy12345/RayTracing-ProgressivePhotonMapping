#include <iostream>
#include "display/paint_board.h"
#include "json/json.h"
#include "render/raytracing.h"
#include <unistd.h>
#include <fstream>
#include <pthread.h>

bool show_graph_flag = true;

int main(int argc, char** args)
{

	std::ifstream ifs("configures/config.json");
	Json::CharReaderBuilder reader;
	Json::Value root;  
	JSONCPP_STRING errs;
	Json::parseFromStream(reader, ifs, &root, &errs);

	RayTracing RT;
	PaintBoard PB;
	RT.accept(root);
	RT.registerPaintBoard(&PB);

	RT.run();
	std::cout<<"HAHA"<<std::endl;
	//PB.update();
	PB.save();
	//PB.display();
}
