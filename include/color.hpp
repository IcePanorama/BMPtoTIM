#ifndef _COLOR_HPP_
#define _COLOR_HPP_

#include <cstdint>

class Color
{
  uint8_t red_value_;
  uint8_t green_value_;
  uint8_t blue_value_;

  friend struct ColorHasher_s;

public:
  Color (uint8_t red_value, uint8_t green_value, uint8_t blue_value);
  bool operator== (const Color &other) const;
};

struct ColorHasher_s
{
  std::size_t
  operator() (const Color &c) const
  {
    std::size_t h = static_cast<std::size_t> (c.red_value_);
    h = (h << 8) | c.green_value_;
    h = (h << 8) | c.blue_value_;
    return h;
  }
};

#endif /* _COLOR_HPP_ */
