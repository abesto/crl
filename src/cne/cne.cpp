#include "cne.h"

#include <variant>

#include "../overloaded.h"

namespace cne {

Node* CauseAndEffect::add_node(Value const& value) {
  auto node = std::make_unique<Node>(this, value);
  auto node_ptr = node.get();
  nodes_.push_back(std::move(node));
  return node_ptr;
}

Node const* CauseAndEffect::add_root(Value const& value) {
  auto node = add_node(value);
  roots_.push_back(node);
  return node;
}

Node const* CauseAndEffect::add_effect(Node* cause, Value const& effect) {
  assert(cause != nullptr);
  assert(cause->belongs_to(this));
  auto node = add_node(effect);
  cause->add_effect(node);
  return node;
}

void CauseAndEffect::reset() {
  roots_.clear();
  nodes_.clear();
}

}  // namespace cne

std::ostream& operator<<(std::ostream& os, cne::Value const& value) {
  std::visit(overloaded{[&os](SDL_Event const&) { os << "SDL Event"; }, [&os](auto const& arg) { os << arg; }}, value);

  return os;
}
