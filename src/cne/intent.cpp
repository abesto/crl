#include "intent.h"

#include <variant>

#include "../overloaded.h"

std::ostream& operator<<(std::ostream& os, const Intent& intent) {
  std::visit(
      overloaded{
          [&os](const intent::Quit&) { os << "Quit"; },
          [&os](const intent::Walk& walk) { os << "Walk(" << walk.vector << ")"; },
      },
      intent);
  return os;
}
