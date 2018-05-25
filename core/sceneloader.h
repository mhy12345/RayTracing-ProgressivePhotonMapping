#ifndef SCENELOADER_H
#define SCENELOADER_H

#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <map>
#include "gobject.h"
class GObject;

class SceneLoader : public std::stringstream {
	private:
		std::string txt;
		std::map<std::string,std::string> mapper;
	public:
		SceneLoader(const char* filename);
		void prepare();
		SceneLoader& operator()(const char* item_name);
		void set_children(std::string name, GObject* obj);
};

#endif
