#pragma once

#include <entt/entt.hpp>

#include "../cne.h"
#include "../components.h"
#include "../context.h"

#define SYSTEM_DECL(name)                \
  namespace systems::name {              \
  void update(entt::registry& registry); \
  }
