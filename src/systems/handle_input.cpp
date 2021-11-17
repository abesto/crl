#include "handle_input.h"

#include <SDL2/SDL.h>

namespace systems::handle_input {

void update(entt::registry& registry) {
  auto& cne = registry.ctx<CauseAndEffect&>();
  const auto& keymap = registry.ctx<const context::keymap::Keymap&>();

  for (auto& [cause, event] : cne.lookup<SDL_Event>()) {
    if (event->type != SDL_KEYDOWN) {
      continue;
    }
    if (auto input_action = keymap.get_action(event->key.keysym.sym); input_action) {
      for (auto&& [player] : registry.view<Player>().each()) {
        // Walk
        std::optional<Coord> vector;
        if (input_action == InputAction::East) {
          vector = Coord{1, 0};
        } else if (input_action == InputAction::West) {
          vector = Coord{-1, 0};
        } else if (input_action == InputAction::North) {
          vector = Coord{0, -1};
        } else if (input_action == InputAction::South) {
          vector = Coord{0, 1};
        }
        if (vector) {
          cne.add_effect(cause, intent::Walk{{player}, vector.value()});
        }

        // Other input_action -> intent translations will go here
      }
    }
  }
}

}  // namespace systems::handle_input
