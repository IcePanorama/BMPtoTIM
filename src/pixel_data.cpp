#include "pixel_data.hpp"

#include <format>
#include <stdexcept>

PixelData::PixelData (
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    const std::vector<std::vector<uint8_t> > &data, uint16_t width,
    uint16_t height, uint16_t frame_buffer_x, uint16_t frame_buffer_y)
    : color_table_ (color_table), data_ (data), x_pos_ (frame_buffer_x),
      y_pos_ (frame_buffer_y), width_ (width), height_ (height)
{
  if (this->x_pos_ > 1023)
    throw std::runtime_error (std::format (
        "Error: given frame buffer x coordinate, {:d}, is out of bounds.",
        this->x_pos_));
  if (this->y_pos_ > 511)
    throw std::runtime_error (std::format (
        "Error: given frame buffer y coordinate, {:d}, is out of bounds.",
        this->y_pos_));
}

void
PixelData::export_pixel_data (std::ofstream &fptr)
{
  this->export_header (fptr);
  this->export_data (fptr);
}

void
PixelData::export_header (std::ofstream &fptr)
{
  const uint32_t pixel_data_size
      = PixelData::PIXEL_DATA_HEADER_SIZE
        + sizeof (uint16_t) * this->height_ * this->width_;
  fptr.write (reinterpret_cast<const char *> (&pixel_data_size),
              sizeof (pixel_data_size));

  fptr.write (reinterpret_cast<const char *> (&this->x_pos_),
              sizeof (this->x_pos_));
  fptr.write (reinterpret_cast<const char *> (&this->y_pos_),
              sizeof (this->y_pos_));

  fptr.write (reinterpret_cast<const char *> (&this->width_),
              sizeof (this->width_));
  fptr.write (reinterpret_cast<const char *> (&this->height_),
              sizeof (this->height_));

  if (fptr.fail ())
    throw std::runtime_error (
        "Error exporting pixel data header to output file.");
}

void
PixelData::export_data (std::ofstream &fptr)
{
  for (const auto &row : this->data_)
    {
      for (size_t i = 0; i + 3 < row.size (); i += 4)
        {
          uint16_t value = row.at (i);
          value |= (row.at (i + 1) << 4);
          value |= (row.at (i + 2) << 8);
          value |= (row.at (i + 3) << 12);
          fptr.write (reinterpret_cast<char *> (&value), sizeof (value));
          if (fptr.fail ())
            throw std::runtime_error (
                "Error exporting pixel data to output file.");
        }
    }
}
