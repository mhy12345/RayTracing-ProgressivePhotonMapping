#include <iostream>
#include "display/paint_board.h"
#include "json/json.h"
#include "render/raytracing.h"
#include "render/progressive_photon_mapping.h"
#include <unistd.h>
#include <fstream>
#include <pthread.h>

bool show_graph_flag = true;

void* show_graph(void* params) {
	Render *PPM = (Render*)params;
	PPM->run();	
	show_graph_flag = false;
	return NULL;
}


int main(int argc, char** args)
{
	std::ifstream ifs("configures/config.json");
	Json::CharReaderBuilder reader;
	Json::Value root;  
	JSONCPP_STRING errs;
	Json::parseFromStream(reader, ifs, &root, &errs);

	Render *PPM = new ProgressivePhotonMapping();
	//Render *PPM = new RayTracing();
	PaintBoard PB;
	PPM->accept(root);
	PPM->registerPaintBoard(&PB);

	pthread_t watcher;
	pthread_create(&watcher,NULL,show_graph,PPM);

	while (show_graph_flag) {
		PB.update();
		PB.save();
		PB.display();
		usleep(100000);
	}

	show_graph_flag = false;
	pthread_join(watcher,NULL);

	PB.update();
	PB.save();
}
