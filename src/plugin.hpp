#include <rack.hpp>

using namespace rack;

extern Plugin* pluginInstance;

extern Model* modelRafsCVAttenuator;

template <typename Base>
struct Rogan6PSLight : Base {
	Rogan6PSLight() {
		this->box.size = mm2px(Vec(100.04, 100.04));
		// this->bgColor = nvgRGBA(0, 0, 0, 0);
		this->borderColor = nvgRGBA(0, 0, 0, 0);
	}
};