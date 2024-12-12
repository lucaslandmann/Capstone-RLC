#ifndef MAINSCREEN_1VIEW_HPP
#define MAINSCREEN_1VIEW_HPP

#include <gui_generated/mainscreen_1_screen/MainScreen_1ViewBase.hpp>
#include <gui/mainscreen_1_screen/MainScreen_1Presenter.hpp>

class MainScreen_1View : public MainScreen_1ViewBase
{
public:
    MainScreen_1View();
    virtual ~MainScreen_1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MAINSCREEN_1VIEW_HPP
