#ifndef DELAYPRESENTER_HPP
#define DELAYPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class DelayView;

class DelayPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    DelayPresenter(DelayView& v);

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

    virtual ~DelayPresenter() {}

private:
    DelayPresenter();

    DelayView& view;
};

#endif // DELAYPRESENTER_HPP
