#ifndef EFFECTSMATRIXVIEW_HPP
#define EFFECTSMATRIXVIEW_HPP

#include <gui_generated/effectsmatrix_screen/EffectsMatrixViewBase.hpp>
#include <gui/effectsmatrix_screen/EffectsMatrixPresenter.hpp>

class EffectsMatrixView : public EffectsMatrixViewBase
{
public:
    EffectsMatrixView();
    virtual ~EffectsMatrixView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // EFFECTSMATRIXVIEW_HPP
