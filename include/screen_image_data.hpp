/**
 *  screen_image_data - the Sony Playstation's TIM file format.
 *  see: "TIM: Screen Image Data", pp. 182-6 in 'File Formats'
 */
#ifndef _SCREEN_IMAGE_DATA_HPP_
#define _SCREEN_IMAGE_DATA_HPP_

#include "color.hpp"

#include <cstdint>
#include <unordered_map>
#include <vector>

/*
typedef enum TIMPixelMode_e
{
  PM_4_BIT_CLUT,
  PM_8_BIT_CLUT,
  PM_15_BIT_DIRECT,
  PM_24_BIT_DIRECT,
  PM_MIXED,
  NUM_PIXEL_MODES
} TIMPixelMode_t;
*/

class TIMImage
{
  // TIMPixelMode_t pixel_mode;

  /* CLUT (Color Look-Up Table) */
  bool has_clut_section;
  uint32_t clut_size; // including the size of `clut_size`.
  uint16_t clut_x_pos;
  uint16_t clut_y_pos;
  uint16_t clut_height;
  uint16_t clut_width;
  std::vector<uint16_t> clut_entries;

  /* Pixel data */
  uint32_t pixel_data_size; // including the size of `pixel_data_size`.
  uint16_t pixel_data_x_pos;
  uint16_t pixel_data_y_pos;
  uint16_t pixel_data_height;
  uint16_t pixel_data_width; // in 16-bit units
  std::vector<uint16_t> frame_buffer_pixel_data;

public:
  TIMImage (
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table);
};

#endif /* _SCREEN_IMAGE_DATA_HPP_ */
