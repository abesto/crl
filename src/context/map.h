#pragma once

#include <vector>

namespace context::map {

struct TileType {
  char glyph;
  bool walkable;
};

const TileType WALL = {'#', false};
const TileType FLOOR = {'.', true};

class Map {
 public:
  Map(int width, int height);

  TileType& at(int x, int y);
  const TileType& at(int x, int y) const;

  int width() const { return width_; }
  int height() const { return height_; }

  bool in_bounds(int x, int y) const;
  bool walkable(int x, int y) const;

 private:
  int width_;
  int height_;
  std::vector<TileType> tiles_;
};

}  // namespace context::map
