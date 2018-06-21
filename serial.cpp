#include <iostream>
#include "display/paintboard.h"
#include "json/json.h"
#include "render/raytracing.h"
#include "glog/logging.h"
#include <unistd.h>
#include <fstream>
#include <pthread.h>

bool show_graph_flag = true;

int main(int argc, char** args)
{
	google::InitGoogleLogging(args[0]);
	FLAGS_log_dir="./logs";

	LOG(INFO) << "hello glog"<<std::endl;  
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
	PB.update();
	PB.display();
	std::cout<<"HAHA"<<std::endl;
	cv::waitKey(0);
}
