#include "object.h"
#include <random>

Object::Object() {
	hash = rand();
}

Object::~Object() {
}
