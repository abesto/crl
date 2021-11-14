#pragma once

#include <variant>

#include "coord.h"

namespace action {

struct Quit {};

struct Walk {
  Coord vector;
};

struct Teleport {
  Coord coord;
};

using Action = std::variant<Quit, Walk, Teleport>;

}  // namespace action
