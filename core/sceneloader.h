#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include "gobject.h"
class GObject;

class SceneLoader{
	private:
		std::map<std::string,std::string> mapper;
	public:
		SceneLoader();
		SceneLoader(std::string txt);
		SceneLoader(const char* filename);
		SceneLoader subScene(const char* item_name);
		std::stringstream& operator()(const char* item_name);
};

#endif
