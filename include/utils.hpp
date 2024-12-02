#ifndef _UTILITIES_HPP_
#define _UTILITIES_HPP_

#include "color.hpp"

#include <cstdint>
#include <fstream>

uint32_t read_uint32_from_file (std::ifstream &f);
Color read_color_from_file (std::ifstream &f);

#endif /* _UTILITIES_HPP_ */
