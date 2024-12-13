#include "bitmap_img.hpp"
#include "screen_image_data.hpp"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

int
main (int argc, char **argv)
{
  if (argc < 2)
    throw std::runtime_error (std::format (
        "Improper usage error: try {} path/to/file.bmp\n", argv[0]));

  const std::string input_filename = argv[1];

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
