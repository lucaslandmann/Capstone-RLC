/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <touchgfx/canvas_widget_renderer/CanvasWidgetRenderer.hpp>
#include <touchgfx/Color.hpp>
#include <images/BitmapDatabase.hpp>

Screen1ViewBase::Screen1ViewBase()
{
    touchgfx::CanvasWidgetRenderer::setupBuffer(canvasBuffer, CANVAS_BUFFER_SIZE);
    
    __background.setPosition(0, 0, 800, 480);
    __background.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    add(__background);

    image1.setXY(0, 0);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_800X480_PUZZLE_ID));
    add(image1);

    circleProgress1.setXY(325, 165);
    circleProgress1.setProgressIndicatorPosition(0, 0, 150, 150);
    circleProgress1.setRange(0, 100);
    circleProgress1.setCenter(75, 75);
    circleProgress1.setRadius(65);
    circleProgress1.setLineWidth(20);
    circleProgress1.setStartEndAngle(0, 360);
    circleProgress1.setCapPrecision(180);
    circleProgress1.setBackground(touchgfx::Bitmap(BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_CIRCLEPROGRESS_BACKGROUNDS_MEDIUM_ID));
    circleProgress1Painter.setColor(touchgfx::Color::getColorFromRGB(0, 240, 255));
    circleProgress1.setPainter(circleProgress1Painter);
    circleProgress1.setValue(60);
    add(circleProgress1);
}

Screen1ViewBase::~Screen1ViewBase()
{
    touchgfx::CanvasWidgetRenderer::resetBuffer();
}

void Screen1ViewBase::setupScreen()
{

}