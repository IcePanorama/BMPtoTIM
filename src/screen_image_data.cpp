#include "screen_image_data.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

/*
 *  Width is div'd by 4 due to the output being in 4 bpp mode, this will need
 *  to be updated in the future to make this more flexible.
 */
TIMImage::TIMImage (
    const std::string &filename,
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    const std::vector<std::vector<uint8_t> > &pixel_array, uint16_t width,
    uint16_t height)
    : filename_ (filename), file (), clut (color_table),
      pixel_data (color_table, pixel_array, (width >> 2), height)
{
  format_filename ();

  file.open (this->filename_, std::ios::binary);
  if (!file.is_open ())
    throw std::runtime_error (
        std::format ("Error opening output file, {}\n", this->filename_));

  this->create_file_header ();
  this->export_file_flags ();

  this->clut.export_clut (this->file);
  this->pixel_data.export_pixel_data (this->file);
}

void
TIMImage::format_filename () noexcept
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

// FIXME: add error checking around file i/o
void
TIMImage::create_file_header (void)
{
  this->file.write (reinterpret_cast<const char *> (&TIMImage::ID_VALUE),
                    sizeof (TIMImage::ID_VALUE));
  this->file.write (reinterpret_cast<const char *> (&TIMImage::VERSION_NUMBER),
                    sizeof (TIMImage::VERSION_NUMBER));
  this->file.seekp (0x2, std::ios::cur);
}

// FIXME: add error checking around file i/o
void
TIMImage::export_file_flags (void)
{
  uint32_t flags = TIMImage::PIXEL_MODE | (1 << (CLUT_FLAG_BIT));
  this->file.write (reinterpret_cast<char *> (&flags), sizeof (flags));
}
