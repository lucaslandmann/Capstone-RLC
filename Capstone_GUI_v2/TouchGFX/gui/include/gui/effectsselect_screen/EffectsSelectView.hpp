#ifndef EFFECTSSELECTVIEW_HPP
#define EFFECTSSELECTVIEW_HPP

#include <gui_generated/effectsselect_screen/EffectsSelectViewBase.hpp>
#include <gui/effectsselect_screen/EffectsSelectPresenter.hpp>

class EffectsSelectView : public EffectsSelectViewBase
{
public:
    EffectsSelectView();
    virtual ~EffectsSelectView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EFFECTSSELECTVIEW_HPP
