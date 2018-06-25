#ifndef PROGRESSIVE_PHOTON_MAPPING_H
#define PROGRESSIVE_PHOTON_MAPPING_H

#include <vector>
#include "../core/light.h"
#include "../core/camera.h"
#include "../core/object.h"
#include "../core/color.h"
#include "scene.h"
#include "json/json.h"
#include "render.h"

class ProgressivePhotonMapping : private Scene, public Render{
	private:
		struct ViewPoint {
			public:
				Vector position;
				double strength;
				ViewPoint(Vector pos,double stgh) : position(pos),strength(stgh) {}
		};
		unsigned **hash_table;
		int max_depth;
		int start_rows;
		int bazier_quality;
		int total_round;
		std::vector<ViewPoint> view_pts;
	public:
		//INIT
		ProgressivePhotonMapping();
		~ProgressivePhotonMapping();
		virtual void accept(const Json::Value& val);

		void run();
		void RayTracing(const Vector& rayO, const Vector& rayD,int depth,double lambda,double dist);
};

#endif
