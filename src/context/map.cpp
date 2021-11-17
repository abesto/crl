#include "map.h"

namespace context::map {

Map::Map(int width, int height) : width_(width), height_(height) {
  tiles_.resize(width_ * height_);
  for (int i = 0; i < width_ * height_; ++i) {
    tiles_[i] = FLOOR;
  }
}

TileType& Map::at(int x, int y) { return tiles_[y * width_ + x]; }
TileType const& Map::at(int x, int y) const { return tiles_[y * width_ + x]; }

bool Map::in_bounds(int x, int y) const { return x >= 0 && x < width_ && y >= 0 && y < height_; }

bool Map::walkable(int x, int y) const { return in_bounds(x, y) && at(x, y).walkable; }

}  // namespace context::map
