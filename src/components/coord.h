#pragma once

#include <ostream>

class Coord {
 public:
  int x;
  int y;
};

Coord operator+(const Coord& a, const Coord& b);
std::ostream& operator<<(std::ostream& os, const Coord& coord);

enum class Heading { North, East, South, West, NorthEast, SouthEast, SouthWest, NorthWest };
