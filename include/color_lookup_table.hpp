#ifndef _COLOR_LOOKUP_TABLE_HPP_
#define _COLOR_LOOKUP_TABLE_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>
#include <unordered_map>

class ColorLookupTable
{
  static constexpr uint16_t CLUT_REQ_X_COORD_MULTIPLE = 16;
  /** see: pg. 183, "TIM: Screen Image Data", in 'File Formats'. */
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
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
      uint16_t frame_buffer_x, uint16_t frame_buffer_y);

  void export_clut (std::ofstream &fptr);
};

#endif /* _COLOR_LOOKUP_TABLE_HPP_ */
