#include "coord.h"

Coord operator+(const Coord& a, const Coord& b) { return Coord{a.x + b.x, a.y + b.y}; }

std::ostream& operator<<(std::ostream& os, const Coord& coord) {
  os << "(" << coord.x << ", " << coord.y << ")";
  return os;
}
