#ifndef _BITMAP_IMAGE_HPP_
#define _BITMAP_IMAGE_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

class BitmapImage
{
  static constexpr uint8_t DATA_START_OFFSET_LOC = 0x0A;
  static constexpr uint8_t BITMAP_DIMENSIONS_LOC = 0x12;
  static constexpr uint8_t BYTES_PER_PIXEL = 3;

  std::ifstream file;
  /**
   *  The offset to where the pixel array can be found.
   *  @see: https://en.wikipedia.org/wiki/BMP_file_format#Bitmap_file_header
   */
  uint32_t data_start;
  uint32_t width;
  uint32_t height;
  /**
   *  "the total number of bytes necessary to store one row of pixels," rounded
   *  up to a multiple of 4 bytes. ((BPP * width) / 32) * 4.
   *  @see: https://en.wikipedia.org/wiki/BMP_file_format#Pixel_storage
   */
  uint32_t row_size;

  /** Verifies that the first two bytes of the file are BM (0x424D). */
  void validate_file (void);
  void process_pixel_array (void);

public:
  // FIXME: make TIM a friend
  const std::string filename_;
  std::unordered_map<Color, uint8_t, ColorHasher_s> color_table;
  std::vector<std::vector<uint8_t> > pixel_array;

  BitmapImage (const std::string &filename);
};

#endif /* _BITMAP_IMAGE_HPP_ */
