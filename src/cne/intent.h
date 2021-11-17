#pragma once

#include <entt/entt.hpp>
#include <ostream>
#include <variant>

#include "../components/coord.h"


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

std::ostream& operator<<(std::ostream& os, const Intent& intent);
