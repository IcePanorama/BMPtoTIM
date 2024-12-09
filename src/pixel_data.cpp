#include "pixel_data.hpp"
#include "frame_buffer.hpp"

#include <format>
#include <stdexcept>

PixelData::PixelData (
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    const std::vector<std::vector<uint8_t> > &data, uint16_t width,
    uint16_t height, uint16_t tpage_x, uint16_t tpage_y)
    : color_table_ (color_table), data_ (data), x_pos_ (tpage_x),
      y_pos_ (tpage_y), width_ (width), height_ (height)
{
  if (this->x_pos_ > FRAME_BUFFER_MAX_X_POSITION)
    throw std::runtime_error (
        std::format ("ERROR: Given frame buffer x coordinate for the TPage "
                     "({:d}) is out of bounds [0..{:d}]",
                     this->x_pos_, FRAME_BUFFER_MAX_X_POSITION));
  else if (this->x_pos_ + this->width_ > FRAME_BUFFER_MAX_X_POSITION)
    throw std::runtime_error (std::format (
        "ERROR: TPage would reach out of bounds with given x position ({:d}) "
        "and width ({:d}) [0..{:d}]",
        this->x_pos_, this->width_, FRAME_BUFFER_MAX_X_POSITION));

  /** see: "GetTPage", PSX Run-Time Library Reference, pg. 306. */
  if (this->x_pos_ % PixelData::TPAGE_REQ_X_COORD_MULTIPLE != 0)
    throw std::runtime_error (std::format (
        "ERROR: Given TPage x position {:d} is not a multiple of {:d}.",
        this->x_pos_, PixelData::TPAGE_REQ_X_COORD_MULTIPLE));

  if (this->y_pos_ > FRAME_BUFFER_MAX_Y_POSITION)
    throw std::runtime_error (
        std::format ("ERROR: Given frame buffer y coordinate, {:d}, is out of "
                     "bounds [0..{:d}]",
                     this->y_pos_, FRAME_BUFFER_MAX_Y_POSITION));
  else if (this->y_pos_ + this->height_ > FRAME_BUFFER_MAX_Y_POSITION)
    throw std::runtime_error (std::format (
        "ERROR: TPage would reach out of bounds with given y position ({:d}) "
        "and height ({:d}) [0..{:d}]",
        this->y_pos_, this->height_, FRAME_BUFFER_MAX_Y_POSITION));

  /** see: "GetTPage", PSX Run-Time Library Reference, pg. 306. */
  if (this->y_pos_ % PixelData::TPAGE_REQ_Y_COORD_MULTIPLE != 0)
    throw std::runtime_error (std::format (
        "ERROR: Given TPage y position {:d} is not a multiple of {:d}.",
        this->x_pos_, PixelData::TPAGE_REQ_Y_COORD_MULTIPLE));
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
