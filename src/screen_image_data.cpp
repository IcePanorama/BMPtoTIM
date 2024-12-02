#include "screen_image_data.hpp"

#include <algorithm>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

TIMImage::TIMImage (
    const std::string &filename,
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &clut)
    : filename_ (filename), file ()
{
  format_filename ();

  file.open (this->filename_, std::ios::binary);
  if (!file.is_open ())
    throw std::runtime_error (
        std::format ("Error opening output file, {}\n", this->filename_));

  std::cout << clut.size () << std::endl;
  this->create_file_header ();
  this->export_file_flags ();
  this->export_clut();
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

void export_clut (const std::unordered_map<Color, uint8_t, ColorHasher_s> &clut)
{
}
