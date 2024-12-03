#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>

uint32_t read_uint32_from_file (std::ifstream &f);
Color read_color_from_file (std::ifstream &f);

inline uint8_t
u8_to_u5 (uint8_t x) noexcept
{
  return static_cast<uint8_t> ((x / 255.0) * 0x1F);
}

#endif /* _UTILITIES_HPP_ */
