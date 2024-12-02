#include "bitmap_img.hpp"
#include "utils.hpp"

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

  this->file.seekg (BitmapImage::DATA_START_OFFSET_LOC);
  this->data_start = read_uint32_from_file (this->file);

  this->file.seekg (BitmapImage::BITMAP_DIMENSIONS_LOC);
  this->width = read_uint32_from_file (this->file);
  this->height = read_uint32_from_file (this->file);

  this->row_size
      = static_cast<uint32_t> (
            (BitmapImage::BITS_PER_PIXEL * this->width + 31.0) / 32.0)
        * 4;
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
