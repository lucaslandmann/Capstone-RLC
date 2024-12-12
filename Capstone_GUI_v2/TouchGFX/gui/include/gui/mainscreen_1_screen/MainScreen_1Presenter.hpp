#ifndef MAINSCREEN_1PRESENTER_HPP
#define MAINSCREEN_1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MainScreen_1View;

class MainScreen_1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    MainScreen_1Presenter(MainScreen_1View& v);

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

    virtual ~MainScreen_1Presenter() {}

private:
    MainScreen_1Presenter();

    MainScreen_1View& view;
};

#endif // MAINSCREEN_1PRESENTER_HPP
