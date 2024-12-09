#include "pixel_data.hpp"

PixelData::PixelData (
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    const std::vector<std::vector<uint8_t> > &data, uint16_t width,
    uint16_t height)
    : color_table_ (color_table), data_ (data), x_pos_ (320), y_pos_ (1),
      width_ (width), height_ (height)
{
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
  // FIXME: need to add checks for failures more often when doing file io
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
        }
    }
}
