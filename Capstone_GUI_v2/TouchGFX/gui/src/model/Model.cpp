#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {

extern bool fxButtonPressed;
extern void leds(char opt);

}

bool Model::getButtonValue()
{
#ifndef SIMULATOR
	return fxButtonPressed;
#else
// Implimentation for the simulator
	//return fxButtonPressed;
	return false;
#endif //Simulator
}

void Model::toggleLed(char selection)
{
#ifndef SIMULATOR
	leds(selection);
#else
	//leds(selection);
#endif
}

Model::Model() : modelListener(0)
{

}

void Model::tick()
{

}
