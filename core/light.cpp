#include "light.h"

void Light :: accept(SceneLoader sl) {
	position.accept(sl.subScene("Position"));
}
