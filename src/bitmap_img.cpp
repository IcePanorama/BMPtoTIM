#include "bitmap_img.hpp"

#include <format>
#include <iostream>
#include <stdexcept>
#include <string>

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
  std::string identifier (2, '\0');
  this->file.read (identifier.data (), identifier.size ());
  if (this->file.fail ())
    throw std::runtime_error (
        std::format ("Error reading from file, {}\n", filename));

  if (identifier != "BM")
    throw std::runtime_error (
        std::format ("Error: The file, {}, is not a bitmap image.", filename));
}
