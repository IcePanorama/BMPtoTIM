#include "bitmap_img.hpp"

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
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << e.what () << std::endl;
    }

  return 0;
}
