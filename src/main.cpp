#include "bitmap_img.hpp"
#include "screen_image_data.hpp"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

int
main (void)
{
  const std::string input_filename = "font.bmp";

  try
    {
      BitmapImage tmp (input_filename);
      TIMImage tim (tmp.filename_, tmp.color_table, tmp.pixel_array, tmp.width,
                    tmp.height);
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << e.what () << std::endl;
    }

  return 0;
}
