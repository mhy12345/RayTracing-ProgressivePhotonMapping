#include <iostream>
#include "display/paint_board.h"
#include "json/json.h"
#include "render/raytracing.h"
#include "render/progressive_photon_mapping.h"
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

	Render* PPM = new RayTracing();
	PaintBoard PB;
	PPM->accept(root);
	PPM->registerPaintBoard(&PB);

	PPM->run();
	std::cout<<"HAHA"<<std::endl;
	//PB.update();
	PB.save();
	//PB.display();
}
