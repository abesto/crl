#pragma once

#include <ostream>
#include <variant>

#include "../components/coord.h"


namespace effect {
struct Moved {
  Coord from;
  Coord to;
};

struct Blocked {
  Coord from;
  Coord to;
};

using Effect = std::variant<Moved, Blocked>;
}  // namespace effect

using effect::Effect;

std::ostream& operator<<(std::ostream& os, const Effect& effect);
