#ifndef _COLOR_LOOKUP_TABLE_HPP_
#define _COLOR_LOOKUP_TABLE_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>
#include <unordered_map>

class ColorLookupTable
{
  /** see: pg. 183. */
  static constexpr uint32_t CLUT_HEADER_SIZE = sizeof (uint32_t) * 3;

  const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table_;

  const uint16_t x_pos_;
  const uint16_t y_pos_;
  const uint16_t width;
  const uint16_t height;

  void export_clut_header (std::ofstream &fptr);
  void create_clut_entries (std::ofstream &fptr);

public:
  ColorLookupTable (
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table);

  void export_clut (std::ofstream &fptr);
};

#endif /* _COLOR_LOOKUP_TABLE_HPP_ */
