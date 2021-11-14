#include "coord.h"

Coord operator+(const Coord& a, const Coord& b) { return Coord{a.x + b.x, a.y + b.y}; }
