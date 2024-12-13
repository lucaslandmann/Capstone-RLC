#ifndef EFFECTSSELECTPRESENTER_HPP
#define EFFECTSSELECTPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class EffectsSelectView;

class EffectsSelectPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    EffectsSelectPresenter(EffectsSelectView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~EffectsSelectPresenter() {}

    //virtual void newCH1ReverbToggleValue(bool button);

    void effectsDelayToggle(int selection);

private:
    EffectsSelectPresenter();

    EffectsSelectView& view;
};

#endif // EFFECTSSELECTPRESENTER_HPP
