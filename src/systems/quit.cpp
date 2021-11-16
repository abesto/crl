#include "quit.h"

#include <SDL2/SDL.h>

namespace systems::quit {

void update(entt::registry& registry) {
  const auto& keymap = registry.ctx<const context::keymap::Keymap&>();

  auto quit_events = registry.ctx<CauseAndEffect&>().find_by_type<SDL_Event>([&](auto const& event) {
    if (event.type == SDL_QUIT) {
      return true;
    }
    if (event.type == SDL_KEYDOWN && keymap.is_action(event.key.keysym.sym, InputAction::Quit)) {
      return true;
    }
    return false;
  });

  if (!quit_events.empty()) {
    registry.ctx<context::state::State&>().quit = true;
  }
}

}  // namespace systems::quit
