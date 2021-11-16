#pragma once

class Coord {
 public:
  int x;
  int y;
};

Coord operator+(const Coord& a, const Coord& b);

enum class Heading { North, East, South, West, NorthEast, SouthEast, SouthWest, NorthWest };
