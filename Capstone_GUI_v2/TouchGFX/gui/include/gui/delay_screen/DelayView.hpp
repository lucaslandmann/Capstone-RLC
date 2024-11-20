#ifndef DELAYVIEW_HPP
#define DELAYVIEW_HPP

#include <gui_generated/delay_screen/DelayViewBase.hpp>
#include <gui/delay_screen/DelayPresenter.hpp>

class DelayView : public DelayViewBase
{
public:
    DelayView();
    virtual ~DelayView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DELAYVIEW_HPP
