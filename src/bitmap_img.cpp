#include "bitmap_img.hpp"
#include "utils.hpp"

#include <format>
#include <iostream>
#include <stdexcept>

BitmapImage::BitmapImage (const std::string &filename)
    : file (filename, std::ios::binary), filename_ (filename)
{
  if (!file.is_open ())
    throw std::runtime_error (
        std::format ("Error opening file, {}", filename));

  this->validate_file ();

  this->file.seekg (BitmapImage::DATA_START_OFFSET_LOC);
  this->data_start = read_uint32_from_file (this->file);

  this->file.seekg (BitmapImage::BITMAP_DIMENSIONS_LOC);
  this->width = read_uint32_from_file (this->file);
  this->height = read_uint32_from_file (this->file);

  this->row_size = ((BitmapImage::BYTES_PER_PIXEL * this->width + 3) / 4) * 4;

  this->pixel_array.resize (this->height);
  this->process_pixel_array ();
}

void
BitmapImage::validate_file ()
{
  std::string identifier (2, '\0');
  this->file.read (identifier.data (), identifier.size ());
  if (this->file.fail ())
    throw std::runtime_error (
        std::format ("Error reading from file, {}\n", this->filename_));

  if (identifier != "BM")
    throw std::runtime_error (std::format (
        "Error: The file, {}, is not a bitmap image.", this->filename_));
}

void
BitmapImage::process_pixel_array (void)
{
  this->file.seekg (this->data_start);

  uint32_t padding
      = this->row_size - (this->width * BitmapImage::BYTES_PER_PIXEL);

  for (auto it = this->pixel_array.rbegin (); it != this->pixel_array.rend ();
       it++)
    {
      for (uint32_t x = 0; x < this->width; x++)
        {
          Color c = read_color_from_file (this->file);

          auto got = this->color_table.find (c);
          if (got == this->color_table.end ())
            this->color_table.insert (
                std::make_pair (c, this->color_table.size ()));

          it->push_back (this->color_table.at (c));
        }

      this->file.seekg (padding, std::ios_base::cur);
    }
}
