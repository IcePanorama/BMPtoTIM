#ifndef _BITMAP_IMAGE_HPP_
#define _BITMAP_IMAGE_HPP_

#include <fstream>
#include <string>

class BitmapImage
{
  const std::string filename_;
  std::ifstream file;

  void validate_file (const std::string &filename);

public:
  BitmapImage (const std::string &filename);
};

#endif /* _BITMAP_IMAGE_HPP_ */
