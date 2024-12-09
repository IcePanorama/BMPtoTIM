/**
 *  screen_image_data - the Sony Playstation's TIM file format.
 *  see: "TIM: Screen Image Data", pp. 182-6 in 'File Formats'
 */
#ifndef _SCREEN_IMAGE_DATA_HPP_
#define _SCREEN_IMAGE_DATA_HPP_

#include "color.hpp"
#include "color_lookup_table.hpp"
#include "pixel_data.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class TIMImage
{
  /** see: pg. 183. */
  static constexpr uint8_t ID_VALUE = 0x10;
  /** see: pg. 183. */
  static constexpr uint8_t VERSION_NUMBER = 0x00;
  /** see: pg. 183. TODO: expand to support multiple modes. */
  static constexpr uint32_t PIXEL_MODE = 0x00; // 4-bit CLUT
  static constexpr uint8_t CLUT_FLAG_BIT = 0x3;

  std::string filename_;
  std::ofstream file;

  ColorLookupTable clut;
  PixelData pixel_data;

  /**
   *  Makes the given filename comform to the 8.3 standard, changes the file
   *  extention to TIM.
   */
  void format_filename (void) noexcept;
  void create_file_header (void);
  void export_file_flags (void);

public:
  /**
   *  If `clut_?`, `tpage_?` isn't provided, this defaults to beneath the
   *  320x240 double buffers for the former and to the right of the double
   *  buffers for the latter.
   */
  TIMImage (
      const std::string &filename,
      const std::unordered_map<Color, uint8_t, ColorHasher_s> &color_table,
      const std::vector<std::vector<uint8_t> > &pixel_array, uint16_t width,
      uint16_t height, uint16_t clut_x = 0, uint16_t clut_y = 480,
      uint16_t tpage_x = 320, uint16_t tpage_y = 0);
};

#endif /* _SCREEN_IMAGE_DATA_HPP_ */
