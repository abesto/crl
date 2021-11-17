#pragma once

#include <SDL2/SDL.h>

#include <queue>
#include <tuple>
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
  std::vector<std::unique_ptr<Node>> nodes_ = {};
  std::vector<Node*> roots_;

  Node* add_node(Value const& value);

 public:
  CauseAndEffect() = default;
  CauseAndEffect(const CauseAndEffect&) = delete;
  CauseAndEffect& operator=(const CauseAndEffect&) = delete;

  size_t size() const { return nodes_.size(); }
  Node const* add_root(Value const& value);
  Node const* add_effect(Node* cause, Value const& effect);
  void reset();

  Nodes const& roots() const { return roots_; }

  // lookup<SDL_Event>() -> vec<(Node, SDL_Event)>
  template <typename T>
  const std::vector<std::tuple<Node*, const T*>> lookup() const {
    std::vector<std::tuple<Node*, const T*>> result;
    std::queue<Node*> queue;

    for (auto& root : roots_) {
      queue.push(root);
    }

    while (!queue.empty()) {
      Node* node = queue.front();
      queue.pop();
      if (const T* p = std::get_if<T>(&node->value())) {
        result.push_back(std::make_tuple(node, p));
      }
      for (Node* child : node->effects()) {
        queue.push(child);
      }
    }
    return result;
  }

  // lookup<Intent, Walk>() -> vec<(Node, Walk)>
  template <typename T0, typename T1>
  const std::vector<std::tuple<Node*, const T1*>> lookup() const {
    std::vector<std::tuple<Node*, const T1*>> result;
    std::queue<Node*> queue;

    for (auto& root : roots_) {
      queue.push(root);
    }

    while (!queue.empty()) {
      Node* node = queue.front();
      queue.pop();
      if (const T0* p0 = std::get_if<T0>(&node->value()); const T1* p1 = std::get_if<T1>(p0)) {
        result.push_back(std::make_tuple(node, p1));
      }
      for (Node* child : node->effects()) {
        queue.push(child);
      }
    }
    return result;
  }
};

}  // namespace cne

using CauseAndEffect = cne::CauseAndEffect;
std::ostream& operator<<(std::ostream& os, cne::Value const& value);
