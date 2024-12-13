#include <gui/effectsselect_screen/EffectsSelectView.hpp>
#include <gui/effectsselect_screen/EffectsSelectPresenter.hpp>

EffectsSelectPresenter::EffectsSelectPresenter(EffectsSelectView& v)
    : view(v)
{

}

void EffectsSelectPresenter::activate()
{

}

void EffectsSelectPresenter::deactivate()
{

}

//void EffectsSelectPresenter::newButtonValue(bool button)
//{
//	view.setButtonState(button);
//}

void EffectsSelectPresenter::effectsDelayToggle(int selection)
{
	model->toggleDelay(selection);
}
