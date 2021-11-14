#pragma once

#include <SDL2/SDL.h>

#include <queue>
#include <variant>

#include "effect.h"
#include "input_action.h"
#include "intent.h"

namespace cne {
using Value = std::variant<SDL_Event, InputAction, Intent, Effect>;

class CauseAndEffect;
class Node;
using Nodes = std::vector<Node*>;

class Node {
 private:
  CauseAndEffect const* cne_;
  Value value_;
  Nodes causes_;
  Nodes effects_;

 public:
  Node(CauseAndEffect const* cne, Value value) : cne_(cne), value_(value){};
  bool belongs_to(CauseAndEffect const* cne) const { return cne_ == cne; }

  Value const& value() const { return value_; }
  Nodes const& causes() const { return causes_; }
  Nodes const& effects() const { return effects_; }

  void add_effect(Node* effect) { effects_.push_back(effect); }
};

class CauseAndEffect {
 private:
  std::vector<std::unique_ptr<Node>> nodes = {};
  std::vector<Node*> roots;

  Node* add_node(Value const& value) {
    auto node = std::make_unique<Node>(this, value);
    auto node_ptr = node.get();
    nodes.push_back(std::move(node));
    return node_ptr;
  }

 public:
  CauseAndEffect() = default;
  CauseAndEffect(const CauseAndEffect&) = delete;
  CauseAndEffect& operator=(const CauseAndEffect&) = delete;

  Node const* add_root(Value const& value) {
    auto node = add_node(value);
    roots.push_back(node);
    return node;
  }

  Node const* add_effect(Node* cause, Value const& effect) {
    assert(cause != nullptr);
    assert(cause->belongs_to(this));
    auto node = add_node(effect);
    cause->add_effect(node);
    return node;
  }

  // TODO do something about nonstandard extension warnings wherever this is used
  // Could be optimized to return an iterator instead of a vector
  template <typename F>
  const std::vector<Node*> find(F& lambda) const {
    std::vector<Node*> result;
    std::queue<Node*> queue;

    for (auto& root : roots) {
      queue.push(root);
    }

    while (!queue.empty()) {
      Node* node = queue.front();
      queue.pop();
      if (lambda(node)) {
        result.push_back(node);
      }
      for (Node* child : node->effects()) {
        queue.push(child);
      }
    }
    return result;
  }

  template <typename T>
  const std::vector<Node*> find_by_type() const {
    return find([](auto const* node) { return std::holds_alternative<T>(node->value()); });
  }

  // TODO do something about nonstandard extension warnings wherever this is used
  template <typename T, typename F>
  const std::vector<Node*> find_by_type(F& lambda) const {
    return find([&](auto const* node) {
      return std::holds_alternative<T>(node->value()) && lambda(std::get<T>(node->value()));
    });
  }

  void reset() {
    roots.clear();
    nodes.clear();
  }
};

}  // namespace cne

using CauseAndEffect = cne::CauseAndEffect;
