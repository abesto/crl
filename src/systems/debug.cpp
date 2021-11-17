#include "debug.h"

#include <iostream>

namespace systems::debug {

void print_node(int depth, cne::Node const* node) {
  std::cout << std::string(depth * 2, ' ') << node->value() << std::endl;
  for (auto const& child : node->effects()) {
    print_node(depth + 1, child);
  }
}

void update(entt::registry& registry) {
  auto const& cne = registry.ctx<CauseAndEffect&>();
  // Skip trivial trees
  if (cne.size() <= 1) {
    return;
  }
  // Print the rest
  for (cne::Node const* node : cne.roots()) {
    print_node(0, node);
  }
  std::cout << "---" << std::endl;
}

}  // namespace systems::debug
