#include "read_input.h"

#include <SDL2/SDL.h>

namespace systems::read_input {

void update(entt::registry& registry) {
  SDL_WaitEvent(nullptr);
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    registry.ctx<tcod::ContextPtr&>()->convert_event_coordinates(event);
    registry.ctx<CauseAndEffect&>().add_root(event);
  }
}

}  // namespace systems::read_input
