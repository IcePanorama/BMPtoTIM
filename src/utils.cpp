#include "utils.hpp"

#include <array>
#include <cstdint>
#include <stdexcept>

uint32_t
read_uint32_from_file (std::ifstream &f)
{
  std::array<uint8_t, 4> bytes;
  f.read (reinterpret_cast<char *> (bytes.data ()), bytes.size ());
  if (f.fail ())
    throw std::runtime_error ("Error reading uint32_t from file.");

  return bytes.at (0) | static_cast<uint32_t> (bytes.at (1) << 8)
         | static_cast<uint32_t> (bytes.at (2) << 16)
         | static_cast<uint32_t> (bytes.at (3) << 24);
}

Color
read_color_from_file (std::ifstream &f)
{
  std::array<uint8_t, 3> bytes;
  f.read (reinterpret_cast<char *> (bytes.data ()), bytes.size ());
  if (f.fail ())
    throw std::runtime_error ("Error reading color from file.");

  return Color (bytes.at (2), bytes.at (1), bytes.at (0));
}
