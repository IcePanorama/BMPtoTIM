#include "bitmap_img.hpp"

#include <array>
#include <cstdint>
#include <format>
#include <iostream>
#include <stdexcept>

BitmapImage::BitmapImage (const std::string &filename)
    : filename_ (filename), file (filename_, std::ios::binary)
{
  if (!file.is_open ())
    throw std::runtime_error (
        std::format ("Error opening file, {}", filename));

  this->validate_file (filename);
}

void
BitmapImage::validate_file (const std::string &filename)
{
  std::array<uint8_t, 2> identifier;
  this->file.read (reinterpret_cast<char *> (identifier.data ()),
                   sizeof (identifier));
  if (this->file.fail ())
    throw std::runtime_error (
        std::format ("Error reading from file, {}\n", filename));

  uint16_t value
      = identifier.at (0) | static_cast<uint16_t> ((identifier.at (1) << 8));
  if (value != 0x4D42)
    throw std::runtime_error (
        std::format ("Error: The file, {}, is not a bitmap image.", filename));
}
