#include "screen_image_data.hpp"
#include "color.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

TIMImage::TIMImage (
    const std::string &filename,
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    uint16_t clut_x, uint16_t clut_y, uint16_t pixel_data_x,
    uint16_t pixel_data_y)
    : filename_ (filename), file (), color_table_ (color_table),
      clut_x_ (clut_x), clut_y_ (clut_y), pixel_data_x_ (pixel_data_x),
      pixel_data_y_ (pixel_data_y)
{
  format_filename ();

  file.open (this->filename_, std::ios::binary);
  if (!file.is_open ())
    throw std::runtime_error (
        std::format ("Error opening output file, {}\n", this->filename_));

  std::cout << this->color_table_.size () << std::endl;
  this->create_file_header ();
  this->export_file_flags ();
  this->export_clut ();
}

void
TIMImage::format_filename ()
{
  if (this->filename_.length () > 8)
    this->filename_ = this->filename_.substr (0, 8);

  size_t ext = this->filename_.find ('.');
  if (ext != std::string::npos)
    this->filename_ = this->filename_.substr (0, ext);
  std::transform (this->filename_.begin (), this->filename_.end (),
                  this->filename_.begin (), toupper);
  this->filename_.append (".TIM");
}

void
TIMImage::create_file_header (void)
{
  this->file.write (reinterpret_cast<const char *> (&TIMImage::ID_VALUE),
                    sizeof (TIMImage::ID_VALUE));
  this->file.write (reinterpret_cast<const char *> (&TIMImage::VERSION_NUMBER),
                    sizeof (TIMImage::VERSION_NUMBER));
  this->file.seekp (0x2, std::ios::cur);
}

void
TIMImage::export_file_flags (void)
{
  uint32_t flags = TIMImage::PIXEL_MODE | (1 << (CLUT_FLAG_BIT));
  this->file.write (reinterpret_cast<char *> (&flags), sizeof (flags));
}

void
TIMImage::export_clut (void)
{
  this->export_clut_header ();
  this->create_clut_entries ();
}

void
TIMImage::export_clut_header (void)
{
  // TODO: expand to support other more colors.
  if (this->color_table_.size () > 16)
    throw std::runtime_error ("Error: More than 16 colors found.");

  // Two colors = 32 bits, 4-bit CLUT has 16 entries.
  const uint32_t clut_size
      = TIMImage::CLUT_HEADER_SIZE + sizeof (uint32_t) * 8;
  this->file.write (reinterpret_cast<const char *> (&clut_size),
                    sizeof (clut_size));

  this->file.write (reinterpret_cast<const char *> (&this->clut_x_),
                    sizeof (this->clut_x_));
  this->file.write (reinterpret_cast<const char *> (&this->clut_y_),
                    sizeof (this->clut_y_));

  const uint16_t clut_width = 16;
  this->file.write (reinterpret_cast<const char *> (&clut_width),
                    sizeof (clut_width));
  const uint16_t clut_height = 1;
  this->file.write (reinterpret_cast<const char *> (&clut_height),
                    sizeof (clut_height));
}

void
TIMImage::create_clut_entries (void)
{
  static const Color TRANSPARENCY (0xFF, 0x00, 0xFF);

  uint8_t cnt = 0;
  for (const auto &pair : this->color_table_)
    {
      Color c = pair.first;
      if (c == TRANSPARENCY)
        {
          this->file.seekp (0x2, std::ios::cur);
        }
      else
        {
          const uint8_t r = u8_to_u5 (c.red_value_);
          const uint8_t g = u8_to_u5 (c.green_value_);
          const uint8_t b = u8_to_u5 (c.blue_value_);
          const uint16_t entry = r | (g << 5) | (b << 10);
          this->file.write (reinterpret_cast<const char *> (&entry),
                            sizeof (entry));
        }
      std::cout << std::format ("{:02X} {:02X} {:02X}\n", c.red_value_,
                                c.green_value_, c.blue_value_);
      cnt++;
    }

  std::cout << std::format ("{:d}\n", cnt);
  this->file.seekp (0x2 * (0xF - cnt), std::ios::cur);
}
