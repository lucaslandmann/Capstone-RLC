#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>

extern "C" {

extern bool ch1ReverbToggle;
extern bool fxButtonPressed;
extern void leds(char opt);
extern void delayOn(int channel);

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

bool Model::getCH1ToggleValue()
{
#ifndef SIMULATOR
	return ch1ReverbToggle;
#else
// Implimentation for the simulator;
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

void Model::toggleDelay(int channel)
{
#ifndef SIMULATOR
	delayOn(channel);
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
