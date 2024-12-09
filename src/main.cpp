#include "bitmap_img.hpp"
#include "screen_image_data.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int
main (void)
{
  const std::string input_filename = "font.bmp";

  try
    {
      BitmapImage input (input_filename);
      TIMImage output (input.get_filename (), input.get_color_table (),
                       input.get_pixel_array (), input.get_width (),
                       input.get_height ());
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << e.what () << std::endl;
    }

  return 0;
}
