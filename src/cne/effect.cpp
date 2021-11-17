#include "effect.h"

#include "../overloaded.h"

std::ostream& operator<<(std::ostream& os, const Effect& effect) {
  std::visit(
      overloaded{
          [&os](const effect::Moved& moved) { os << "Moved(" << moved.from << " to " << moved.to << ")"; },
          [&os](const effect::Blocked& blocked) { os << "Blocked(" << blocked.from << " to " << blocked.to << ")"; },
      },
      effect);
  return os;
}
