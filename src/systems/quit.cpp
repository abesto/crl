#include "quit.h"

#include <SDL2/SDL.h>

namespace systems::quit {

void update(entt::registry& registry) {
  const auto& cne = registry.ctx<CauseAndEffect&>();
  const auto& keymap = registry.ctx<const context::keymap::Keymap&>();

  bool should_quit = false;

  for (const auto& [_, event] : cne.lookup<SDL_Event>()) {
    if (event->type == SDL_QUIT) {
      should_quit = true;
      break;
    }

    if (event->type == SDL_KEYDOWN && keymap.is_action(event->key.keysym.sym, InputAction::Quit)) {
      should_quit = true;
      break;
    }
  }

  if (should_quit) {
    registry.ctx<context::state::State&>().quit = true;
  }
}

}  // namespace systems::quit
