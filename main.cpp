#include <iostream>
#include "display/paint_board.h"
#include "json/json.h"
#include "render/raytracing.h"
#include <unistd.h>
#include <fstream>
#include <pthread.h>

bool show_graph_flag = true;

void* show_graph(void* params) {
	RayTracing *RT = (RayTracing*)params;
	RT->run();	
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

	RayTracing RT;
	PaintBoard PB;
	RT.accept(root);
	RT.registerPaintBoard(&PB);

	pthread_t watcher;
	pthread_create(&watcher,NULL,show_graph,&RT);

	while (show_graph_flag) {
		PB.update();
        std::cout<<"Saving..."<<std::endl;
		PB.save_raw();
        std::cout<<"Saved..."<<std::endl;
		//PB.display();
		usleep(1000000);
	}

	show_graph_flag = false;
	pthread_join(watcher,NULL);

	PB.update();
	PB.save();
}
