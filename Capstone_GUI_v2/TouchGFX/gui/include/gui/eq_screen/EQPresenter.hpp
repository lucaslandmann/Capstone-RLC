#ifndef EQPRESENTER_HPP
#define EQPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class EQView;

class EQPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    EQPresenter(EQView& v);

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

    virtual ~EQPresenter() {}

private:
    EQPresenter();

    EQView& view;
};

#endif // EQPRESENTER_HPP
