/**
 *  screen_image_data - the Sony Playstation's TIM file format.
 *  see: "TIM: Screen Image Data", pp. 182-6 in 'File Formats'
 */
#ifndef _SCREEN_IMAGE_DATA_HPP_
#define _SCREEN_IMAGE_DATA_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>

class TIMImage
{
  /** see: pg. 183. */
  static constexpr uint8_t ID_VALUE = 0x10;
  /** see: pg. 183. */
  static constexpr uint8_t VERSION_NUMBER = 0x00;
  /** see: pg. 183. TODO: expand to support multiple modes. */
  static constexpr uint32_t PIXEL_MODE = 0x00; // 4-bit CLUT
  static constexpr uint8_t CLUT_FLAG_BIT = 0x3;
  /** see: pg. 183. */
  static constexpr uint32_t CLUT_HEADER_SIZE = sizeof (uint32_t) * 3;

  std::string filename_;
  std::ofstream file;

  const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table_;

  const uint16_t clut_x_;
  const uint16_t clut_y_;
  const uint16_t pixel_data_x_;
  const uint16_t pixel_data_y_;

  /**
   *  Makes the given filename comform to the 8.3 standard, changes the file
   *  extention to TIM.
   */
  void format_filename (void);
  void create_file_header (void);
  void export_file_flags (void);
  /**
   *  `dx`, `dy` determines the location of the CLUT in the frame buffer.
   *  see: pg. 183.
   */
  void export_clut (void);
  void export_clut_header (void);
  void create_clut_entries (void);

public:
  TIMImage (
      const std::string &filename,
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
      uint16_t clut_x, uint16_t clut_y, uint16_t pixel_data_x,
      uint16_t pixel_data_y);
};

#endif /* _SCREEN_IMAGE_DATA_HPP_ */
