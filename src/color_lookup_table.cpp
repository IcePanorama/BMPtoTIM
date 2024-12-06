#include "color_lookup_table.hpp"
#include "utils.hpp"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <format>
#include <iostream>
#include <numeric>
#include <vector>

ColorLookupTable::ColorLookupTable (
    const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table)
    : color_table_ (color_table), x_pos_ (320), y_pos_ (0), width (16),
      height (1)
{
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
    throw std::runtime_error ("Error: More than 16 colors found.");

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
}

void
ColorLookupTable::create_clut_entries (std::ofstream &fptr)
{
  static const Color TRANSPARENCY (0xFF, 0x00, 0xFF);

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
        }
      else
        {
          const uint8_t r = u8_to_u5 (color.red_value_);
          const uint8_t g = u8_to_u5 (color.green_value_);
          const uint8_t b = u8_to_u5 (color.blue_value_);
          const uint16_t entry = r | (g << 5) | (b << 10);
          fptr.write (reinterpret_cast<const char *> (&entry), sizeof (entry));
        }
    }
  fptr.seekp (0x2 * (0x10 - cnt), std::ios::cur);
}
