#include "walk.h"

namespace systems::walk {

void update(entt::registry& registry) {
  auto& cne = registry.ctx<CauseAndEffect>();
  auto walk_nodes =
      cne.find_by_type<Intent>([](auto const& intent) { return std::holds_alternative<intent::Walk>(intent); });

  for (auto&& cause : walk_nodes) {
    auto intent = std::get<Intent>(cause->value());
    auto walk = std::get<intent::Walk>(intent);
    auto& position = registry.get<Coord>(walk.actor);
    auto candidate_position = position + walk.vector;
    if (candidate_position.x >= 0 && candidate_position.x < 80 && candidate_position.y >= 0 &&
        candidate_position.y < 25) {
      registry.replace<Coord>(walk.actor, candidate_position);
      cne.add_effect(cause, effect::Moved{position, candidate_position});
    }
  }
}

}  // namespace systems::walk
