// 4.24.1 0xc9d5cfcd
// Generated by imageconverter. Please, do not edit!

#include <images/BitmapDatabase.hpp>
#include <touchgfx/Bitmap.hpp>

extern const unsigned char image_alternate_theme_images_backgrounds_800x480_puzzle[]; // BITMAP_ALTERNATE_THEME_IMAGES_BACKGROUNDS_800X480_PUZZLE_ID = 0, Size: 800x480 pixels
extern const unsigned char image_alternate_theme_images_widgets_circleprogress_backgrounds_medium[]; // BITMAP_ALTERNATE_THEME_IMAGES_WIDGETS_CIRCLEPROGRESS_BACKGROUNDS_MEDIUM_ID = 1, Size: 150x150 pixels

const touchgfx::Bitmap::BitmapData bitmap_database[] = {
    { image_alternate_theme_images_backgrounds_800x480_puzzle, 0, 800, 480, 0, 0, 800, ((uint8_t)touchgfx::Bitmap::RGB888) >> 3, 480, ((uint8_t)touchgfx::Bitmap::RGB888) & 0x7 },
    { image_alternate_theme_images_widgets_circleprogress_backgrounds_medium, 0, 150, 150, 130, 43, 13, ((uint8_t)touchgfx::Bitmap::ARGB8888) >> 3, 64, ((uint8_t)touchgfx::Bitmap::ARGB8888) & 0x7 }
};

namespace BitmapDatabase
{
const touchgfx::Bitmap::BitmapData* getInstance()
{
    return bitmap_database;
}

uint16_t getInstanceSize()
{
    return (uint16_t)(sizeof(bitmap_database) / sizeof(touchgfx::Bitmap::BitmapData));
}
} // namespace BitmapDatabase