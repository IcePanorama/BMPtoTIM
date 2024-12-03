#ifndef _PIXEL_DATA_HPP_
#define _PIXEL_DATA_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>
#include <unordered_map>
#include <vector>

class PixelData
{
  static constexpr uint32_t PIXEL_DATA_HEADER_SIZE = sizeof (uint32_t) * 3;

  const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table_;
  const std::vector<std::vector<uint8_t> > &data_;

  const uint16_t x_pos_;
  const uint16_t y_pos_;
  const uint16_t width_;
  const uint16_t height_;

  void export_header (std::ofstream &fptr);
  void export_data (std::ofstream &fptr);

public:
  PixelData (
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
      const std::vector<std::vector<uint8_t> > &data, uint16_t width,
      uint16_t height);

  void export_pixel_data (std::ofstream &fptr);
};

#endif /* _PIXEL_DATA_HPP_ */
