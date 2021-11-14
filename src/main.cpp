#include <SDL2/SDL.h>
#include <libtcod.h>

#include <optional>
#include <variant>

#include "action.h"
#include "keymap.h"

int main(int argc, char* argv[]) {
  auto console = tcod::Console{80, 25};  // Main console.

  // Configure the context.
  auto params = TCOD_ContextParams{};
  params.tcod_version = TCOD_COMPILEDVERSION;
  params.console = console.get();
  params.window_title = "Libtcod Project";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;
  params.argv = argv;

  auto context = tcod::new_context(params);

  Coord position{40, 12};
  auto current_keymap = keymap::main;
  bool quit = false;

  while (!quit) {  // Game loop.
    // Render current state
    console.clear();
    console.at(position.x, position.y).ch = '@';
    context->present(console);

    // Wait for input
    SDL_Event event;
    SDL_WaitEvent(nullptr);  // Optional, sleep until events are available.

    // Handle all the events we got
    while (SDL_PollEvent(&event)) {
      context->convert_event_coordinates(event);  // Optional, converts pixel coordinates into tile coordinates.

      if (event.type == SDL_QUIT) {
        quit = true;
        continue;
      }

      // input_action = keymap[input]
      if (event.type == SDL_KEYDOWN) {
        if (auto it = current_keymap.find(event.key.keysym.sym); it != current_keymap.end()) {
          auto input_action = it->second;

          // input_action -> action
          std::optional<action::Action> action;
          if (input_action == InputAction::East) {
            action = action::Walk{1, 0};
          } else if (input_action == InputAction::West) {
            action = action::Walk{-1, 0};
          } else if (input_action == InputAction::North) {
            action = action::Walk{0, -1};
          } else if (input_action == InputAction::South) {
            action = action::Walk{0, 1};
          } else if (input_action == InputAction::Quit) {
            quit = true;
          }

          // Handle action: walking
          if (action != std::nullopt && std::holds_alternative<action::Walk>(*action)) {
            auto candidate_position = position + std::get<action::Walk>(*action).vector;
            if (candidate_position.x >= 0 && candidate_position.x < 80 && candidate_position.y >= 0 &&
                candidate_position.y < 25) {
              position = candidate_position;
            }
          }
        }
      }
    }
  }
  return 0;
}
