#pragma once

#include <entt/entt.hpp>
#include <variant>

#include "../coord.h"

namespace intent {

struct HasActor {
  entt::entity actor;
};

struct Quit {};

struct Walk : HasActor {
  Coord vector;
};

using Intent = std::variant<Quit, Walk>;
}  // namespace intent

using Intent = intent::Intent;
