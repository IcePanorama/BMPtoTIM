#include "color_lookup_table.hpp"
#include "frame_buffer.hpp"
#include "utils.hpp"

#include <algorithm>
#include <format>
#include <numeric>
#include <stdexcept>
#include <vector>

ColorLookupTable::ColorLookupTable (
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
    uint16_t clut_x, uint16_t clut_y)
    : color_table_ (color_table), x_pos_ (clut_x), y_pos_ (clut_y), width (16),
      height (1)
{
  if (this->x_pos_ > FRAME_BUFFER_MAX_X_POSITION)
    throw std::runtime_error (
        std::format ("ERROR: Given frame buffer x coordinate for the CLUT "
                     "({:d}) is out of bounds [0..{:d}]",
                     this->x_pos_, FRAME_BUFFER_MAX_X_POSITION));
  else if (this->x_pos_ + this->width > FRAME_BUFFER_MAX_X_POSITION)
    throw std::runtime_error (std::format (
        "ERROR: CLUT would reach out of bounds with given x position ({:d}) "
        "and width ({:d}) [0..{:d}]",
        this->x_pos_, this->width, FRAME_BUFFER_MAX_X_POSITION));

  /** see: "GetClut", PSX Run-Time Library Reference, pg. 295. */
  if (this->x_pos_ % ColorLookupTable::CLUT_REQ_X_COORD_MULTIPLE != 0)
    throw std::runtime_error (std::format (
        "ERROR: Given frame buffer x position {:d} is not a multiple of {:d}.",
        this->x_pos_, ColorLookupTable::CLUT_REQ_X_COORD_MULTIPLE));

  if (this->y_pos_ > FRAME_BUFFER_MAX_Y_POSITION)
    throw std::runtime_error (
        std::format ("ERROR: Given frame buffer y coordinate, {:d}, is out of "
                     "bounds [0..{:d}]",
                     this->y_pos_, FRAME_BUFFER_MAX_Y_POSITION));
  else if (this->y_pos_ + this->height > FRAME_BUFFER_MAX_Y_POSITION)
    throw std::runtime_error (std::format (
        "ERROR: CLUT would reach out of bounds with given y position ({:d}) "
        "and height ({:d}) [0..{:d}]",
        this->y_pos_, this->height, FRAME_BUFFER_MAX_Y_POSITION));
}

void
ColorLookupTable::export_clut (std::ofstream &fptr)
{
  this->export_clut_header (fptr);
  this->create_clut_entries (fptr);
}

void
ColorLookupTable::export_clut_header (std::ofstream &fptr)
{
  // TODO: expand to support other more colors.
  if (this->color_table_.size () > 16)
    throw std::runtime_error ("ERROR: More than 16 colors found.");

  // Two colors = 32 bits, 4-bit CLUT has 16 entries.
  const uint32_t clut_size
      = ColorLookupTable::CLUT_HEADER_SIZE + sizeof (uint32_t) * 8;
  fptr.write (reinterpret_cast<const char *> (&clut_size), sizeof (clut_size));

  fptr.write (reinterpret_cast<const char *> (&this->x_pos_),
              sizeof (this->x_pos_));
  fptr.write (reinterpret_cast<const char *> (&this->y_pos_),
              sizeof (this->y_pos_));

  const uint16_t clut_width = 16;
  fptr.write (reinterpret_cast<const char *> (&clut_width),
              sizeof (clut_width));
  const uint16_t clut_height = 1;
  fptr.write (reinterpret_cast<const char *> (&clut_height),
              sizeof (clut_height));

  if (fptr.fail ())
    throw std::runtime_error ("ERROR: Error exporting CLUT header.");
}

void
ColorLookupTable::create_clut_entries (std::ofstream &fptr)
{
  static const Color TRANSPARENCY (0xFF, 0x00, 0xFF);
  constexpr const char *ERR_MSG = "ERROR: Error exporting CLUT entries.";

  // Extract key-values from color table
  std::vector<Color> keys;
  std::vector<uint8_t> values;
  for (const auto &kv : this->color_table_)
    {
      keys.push_back (kv.first);
      values.push_back (kv.second);
    }

  // Sort colors based on their associated values.
  std::vector<size_t> indices (values.size ());
  std::iota (indices.begin (), indices.end (), 0);
  std::sort (indices.begin (), indices.end (), [&] (size_t a, size_t b) {
    return values.at (a) < values.at (b);
  });

  std::vector<Color> sorted_keys (keys);
  std::vector<uint8_t> sorted_values (values.size ());
  for (size_t i = 0; i < indices.size (); i++)
    {
      sorted_keys[i] = keys.at (indices.at (i));
      sorted_values[i] = values.at (indices.at (i));
    }

  /* Export clut entries. */
  uint8_t cnt;
  for (cnt = 0; cnt < sorted_keys.size (); cnt++)
    {
      const Color &color = sorted_keys.at (cnt);
      if (color == TRANSPARENCY)
        {
          fptr.seekp (0x2, std::ios::cur);
          if (fptr.fail ())
            throw std::runtime_error (ERR_MSG);
        }
      else
        {
          const uint8_t r = u8_to_u5 (color.get_red_value ());
          const uint8_t g = u8_to_u5 (color.get_green_value ());
          const uint8_t b = u8_to_u5 (color.get_blue_value ());
          const uint16_t entry = r | (g << 5) | (b << 10);
          fptr.write (reinterpret_cast<const char *> (&entry), sizeof (entry));
          if (fptr.fail ())
            throw std::runtime_error (ERR_MSG);
        }
    }
  fptr.seekp (0x2 * (0x10 - cnt), std::ios::cur);
  if (fptr.fail ())
    throw std::runtime_error (ERR_MSG);
}
