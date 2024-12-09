#include "color.hpp"

Color::Color (uint8_t red_value, uint8_t green_value, uint8_t blue_value)
    : red_value_ (red_value), green_value_ (green_value),
      blue_value_ (blue_value)
{
}

bool
Color::operator== (const Color &other) const
{
  return this->red_value_ == other.red_value_
         && this->green_value_ == other.green_value_
         && this->blue_value_ == other.blue_value_;
}

uint8_t
Color::get_red_value (void) const
{
  return this->red_value_;
}

uint8_t
Color::get_green_value (void) const
{
  return this->green_value_;
}

uint8_t
Color::get_blue_value (void) const
{
  return this->blue_value_;
}
