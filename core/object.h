#ifndef OBJECT_H
#define OBJECT_H

#include "json/json.h"
#include "color.h"
#include "vector.h"
#include "texture.h"
class Object;

struct Collision {
	Vector C;//The Center of Collision
	Vector N;//The normal of the plane
	Vector I;//The direction of reflaction
	double dist;
	bool face;
	Object* belongs;
	std::string description()const;
	void refraction(Vector& resO, Vector& resD)const;
	void reflection(Vector& resO, Vector& resD)const;
	Vector getSurfaceC()const;
	Vector getBackfaceC()const;
};

struct Material {
	double refl;//reflection ratio
	double diff;//diffusion ratio
	double spec;//high light diffusion
	double refr;//refraction ratio
	double refr_k;
	void accept(const Json::Value& val);
};

class Object {
	protected:
		unsigned hash;
		Material material;
		std::string name;
		Texture* texture;
		Texture* absorb;
	public:
		Object();
		virtual ~Object();
		virtual void accept(const Json::Value& val);
		unsigned getHash()const {return hash;}

		const Material& getMaterial()const {return material;}

		virtual bool collideWith(const Vector& rayO, const Vector& rayD,Collision& coll) = 0;
		virtual Color getColor(const Vector &pos)const = 0;
		std::string getName()const{return name;}
		const Texture& getAbsorb()const;
};


#endif
