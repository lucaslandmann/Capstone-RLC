#ifndef REVERBVIEW_HPP
#define REVERBVIEW_HPP

#include <gui_generated/reverb_screen/ReverbViewBase.hpp>
#include <gui/reverb_screen/ReverbPresenter.hpp>

class ReverbView : public ReverbViewBase
{
public:
    ReverbView();
    virtual ~ReverbView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // REVERBVIEW_HPP
