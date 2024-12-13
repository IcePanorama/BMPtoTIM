#include "bitmap_img.hpp"
#include "screen_image_data.hpp"

#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

static uint16_t clut_x_ = 0;
static uint16_t clut_y_ = 480;
static uint16_t tpage_x_ = 320;
static uint16_t tpage_y_ = 0;

static void process_command_line_args (int argc, char **argv);
static void set_clut_tpage_x_y (uint16_t clut_x, uint16_t clut_y,
                                uint16_t tpage_x, uint16_t tpage_y);

int
main (int argc, char **argv)
{
  process_command_line_args (argc, argv);

  const std::string input_filename = argv[1];

  try
    {
      BitmapImage input (input_filename);
      TIMImage output (input.get_filename (), input.get_color_table (),
                       input.get_pixel_array (), input.get_width (),
                       input.get_height (), clut_x_, clut_y_, tpage_x_,
                       tpage_y_);
    }
  catch (const std::runtime_error &e)
    {
      std::cerr << e.what () << std::endl;
    }

  return 0;
}

void
process_command_line_args (int argc, char **argv)
{
  if (argc < 2)
    throw std::runtime_error (std::format (
        "Improper usage error: try {} path/to/file.bmp\n", argv[0]));
  else if (argc > 2)
    {
      if (argc < 6)
        throw std::runtime_error (
            std::format ("Improper usage error: try {} path/to/file.bmp "
                         "[clut_x_] [clut_y_] [tpage_x_] [tpage_y_]\n",
                         argv[0]));
      set_clut_tpage_x_y (std::stoi (argv[2]), std::stoi (argv[3]),
                          std::stoi (argv[4]), std::stoi (argv[5]));
    }
}

void
set_clut_tpage_x_y (uint16_t clut_x, uint16_t clut_y, uint16_t tpage_x,
                    uint16_t tpage_y)
{
  clut_x_ = clut_x;
  clut_y_ = clut_y;
  tpage_x_ = tpage_x;
  tpage_y_ = tpage_y;
}
