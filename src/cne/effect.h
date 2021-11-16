#pragma once

#include <variant>

#include "../components/coord.h"

namespace effect {
struct Moved {
  Coord from;
  Coord to;
};

using Effect = std::variant<Moved>;
}  // namespace effect

using effect::Effect;
