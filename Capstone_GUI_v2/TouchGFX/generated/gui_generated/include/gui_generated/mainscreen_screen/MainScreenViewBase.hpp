/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef MAINSCREENVIEWBASE_HPP
#define MAINSCREENVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/mainscreen_screen/MainScreenPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/Image.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/widgets/TextureMapper.hpp>
#include <touchgfx/containers/Slider.hpp>

class MainScreenViewBase : public touchgfx::View<MainScreenPresenter>
{
public:
    MainScreenViewBase();
    virtual ~MainScreenViewBase();
    virtual void setupScreen();

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Image image1;
    touchgfx::Image image1_1;
    touchgfx::Image image1_2;
    touchgfx::Image image1_3;
    touchgfx::TextArea textArea2_2_4;
    touchgfx::TextArea textArea2_2_3;
    touchgfx::TextArea textArea2_2_2;
    touchgfx::TextArea textArea2_2_1;
    touchgfx::TextArea textArea2_1_4;
    touchgfx::TextArea textArea2_1_5;
    touchgfx::TextArea textArea2_1_3;
    touchgfx::TextArea textArea2_1_2;
    touchgfx::TextArea textArea2_1_1;
    touchgfx::TextArea textArea2_2;
    touchgfx::TextArea textArea2_1;
    touchgfx::Button button2_5;
    touchgfx::Button button2_3;
    touchgfx::Button button2_4;
    touchgfx::Button button2_2;
    touchgfx::Button button2_1;
    touchgfx::Button button1_5;
    touchgfx::Button button1_4;
    touchgfx::Button button1_3;
    touchgfx::Button button1_2;
    touchgfx::Button button1_1;
    touchgfx::TextureMapper textureMapper1_4;
    touchgfx::TextureMapper textureMapper1_5;
    touchgfx::TextureMapper textureMapper1_3;
    touchgfx::TextureMapper textureMapper1_2;
    touchgfx::TextArea textArea1_1_1;
    touchgfx::TextArea textArea1_1_3;
    touchgfx::TextArea textArea1_2;
    touchgfx::TextArea textArea1_1;
    touchgfx::TextArea textArea1_1_4;
    touchgfx::Slider slider1_5;
    touchgfx::Slider slider1_6;
    touchgfx::Slider slider1_3;
    touchgfx::TextArea textArea1_1_2;
    touchgfx::Slider slider1_4;
    touchgfx::Slider slider1_2;
    touchgfx::Slider slider1;
    touchgfx::Slider slider1_1;
    touchgfx::TextArea RLCHeader;
    touchgfx::TextArea textArea1;
    touchgfx::TextureMapper Ch1PanKnob;
    touchgfx::TextureMapper CH2PanKnob;
    touchgfx::Button CH1Mute;
    touchgfx::Button EXSendButton1;
    touchgfx::TextArea CH1EffectsSendText;

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<MainScreenViewBase, const touchgfx::AbstractButton&> buttonCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);

};

#endif // MAINSCREENVIEWBASE_HPP