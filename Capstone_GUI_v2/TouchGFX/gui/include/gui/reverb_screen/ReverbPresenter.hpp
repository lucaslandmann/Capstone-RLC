#ifndef REVERBPRESENTER_HPP
#define REVERBPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ReverbView;

class ReverbPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ReverbPresenter(ReverbView& v);

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

    virtual ~ReverbPresenter() {}

private:
    ReverbPresenter();

    ReverbView& view;
};

#endif // REVERBPRESENTER_HPP
