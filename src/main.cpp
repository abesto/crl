#include <SDL2/SDL.h>
#include <libtcod.h>

#include <entt/entt.hpp>
#include <optional>
#include <variant>

#include "action.h"
#include "keymap.h"

struct State {
  bool quit;
};

struct Player {};

struct Renderable {
  char glyph;
};

void render(entt::registry& registry) {
  auto& console = registry.ctx<tcod::Console>();
  auto& context = registry.ctx<tcod::ContextPtr>();

  console.clear();
  for (auto&& [entity, renderable, position] : registry.view<Renderable, Coord>().each()) {
    console.at(position.x, position.y).ch = renderable.glyph;
  }
  context->present(console);
}

void wait_for_input() { SDL_WaitEvent(nullptr); }

bool input(entt::registry& registry) {
  // Wait for input
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    registry.ctx<tcod::ContextPtr>()->convert_event_coordinates(event);
    registry.set<const SDL_Event>(event);
    return true;
  } else {
    registry.unset<SDL_Event>();
    return false;
  }
}

void handle_quit_request(entt::registry& registry) {
  if (auto* event = registry.try_ctx<SDL_Event>()) {
    if (event->type == SDL_QUIT) {
      registry.ctx<State>().quit = true;
    } else if (event->type == SDL_KEYDOWN) {
      auto keymap = registry.ctx<keymap::Keymap>();
      if (keymap.is_action(event->key.keysym.sym, InputAction::Quit)) {
        registry.ctx<State>().quit = true;
      }
    }
  }
}

void handle_input(entt::registry& registry) {
  // Translate InputAction to Action
  std::optional<action::Action> action;
  if (auto* event = registry.try_ctx<const SDL_Event>(); event != nullptr && event->type == SDL_KEYDOWN) {
    auto keymap = registry.ctx<keymap::Keymap>();
    if (auto input_action = keymap.get_action(event->key.keysym.sym); input_action) {
      if (input_action == InputAction::East) {
        action = action::Walk{1, 0};
      } else if (input_action == InputAction::West) {
        action = action::Walk{-1, 0};
      } else if (input_action == InputAction::North) {
        action = action::Walk{0, -1};
      } else if (input_action == InputAction::South) {
        action = action::Walk{0, 1};
      }
    }
  }

  // Assign the action to any player characters
  if (action) {
    for (auto&& [entity] : registry.view<Player>().each()) {
      registry.emplace<action::Action>(entity, *action);
    }
  }

  // Clean up
  registry.unset<SDL_Event>();
}

void walk(entt::registry& registry) {
  for (auto&& [entity, position, action] : registry.view<Coord, action::Action>().each()) {
    if (std::holds_alternative<action::Walk>(action)) {
      auto candidate_position = position + std::get<action::Walk>(action).vector;
      if (candidate_position.x >= 0 && candidate_position.x < 80 && candidate_position.y >= 0 &&
          candidate_position.y < 25) {
        registry.replace<Coord>(entity, candidate_position);
      }
      registry.erase<action::Action>(entity);
    }
  }
}

int main(int argc, char* argv[]) {
  entt::registry registry;
  registry.set<tcod::Console>(80, 25);

  // Configure the context.
  auto params = TCOD_ContextParams{};
  params.tcod_version = TCOD_COMPILEDVERSION;
  params.console = registry.ctx<tcod::Console>().get();
  params.window_title = "crl";
  params.sdl_window_flags = SDL_WINDOW_RESIZABLE;
  params.vsync = true;
  params.argc = argc;
  params.argv = argv;
  auto context = tcod::new_context(params);

  registry.set<tcod::ContextPtr&>(context);

  const auto player = registry.create();
  registry.emplace<Player>(player);
  registry.emplace<Coord>(player, 40, 12);
  registry.emplace<Renderable>(player, '@');

  registry.set<keymap::Keymap>(keymap::main);
  registry.set<State>(false);

  while (!registry.ctx<const State>().quit) {  // Game loop.
    render(registry);
    wait_for_input();
    while (input(registry)) {
      handle_quit_request(registry);
      handle_input(registry);
      walk(registry);
    }
  }

  return 0;
}
