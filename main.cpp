#include <iostream>
#include "display/PaintBoard.h"
#include "json/json.h"
#include "render/raytracing.h"
#include "glog/logging.h"
#include <unistd.h>
#include <fstream>


int main(int argc, char** args)
{
	google::InitGoogleLogging(args[0]);
	FLAGS_logtostderr = 1; 

	LOG(INFO) << "hello glog"<<std::endl;  
	std::ifstream ifs("configures/config.json");
	Json::CharReaderBuilder reader;
	Json::Value root;  
	JSONCPP_STRING errs;
	Json::parseFromStream(reader, ifs, &root, &errs);

	RayTracing RT;
	RT.accept(root);
	RT.run();
}
