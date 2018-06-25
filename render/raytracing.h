#ifndef RAYTRACING_H
#define RAYTRACING_H

#include <vector>
#include "../core/light.h"
#include "../core/object.h"
#include "../core/color.h"
#include "scene.h"
#include "json/json.h"
#include "render.h"

class RayTracing : private Scene,public Render{
	private:
		unsigned **hash_table;
		int max_depth;
		int shade_quality;
		int spec_power;
		int start_rows;
		int bazier_quality;
	public:
		//INIT
		RayTracing();
		~RayTracing();
		virtual void accept(const Json::Value& val);


		//RayTrace
		Color calcReflection(const Object& obj, const Collision& obj_coll, int depth, unsigned& hash);
		Color calcRefraction(const Object& obj, const Collision& obj_coll, int depth, unsigned& hash);
		Color calcDiffusion(const Object& obj, const Collision& obj_coll);

		Color rayTrace(const Vector& rayO, const Vector& rayD, int depth, unsigned& hash);

		void run();
};

#endif
