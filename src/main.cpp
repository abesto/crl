#include <SDL2/SDL.h>
#include <libtcod.h>

#include <entt/entt.hpp>
#include <optional>
#include <variant>

#include "cne/cne.h"
#include "cne/effect.h"
#include "cne/intent.h"
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
  SDL_Event event;
  if (SDL_PollEvent(&event)) {
    registry.ctx<tcod::ContextPtr&>()->convert_event_coordinates(event);
    registry.ctx<CauseAndEffect&>().add_root(event);
    return true;
  } else {
    return false;
  }
}

void handle_quit_request(entt::registry& registry) {
  auto keymap = registry.ctx<const keymap::Keymap&>();

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
    registry.ctx<State&>().quit = true;
  }
}

void handle_input(entt::registry& registry) {
  auto& cne = registry.ctx<CauseAndEffect>();
  auto input_nodes = cne.find_by_type<SDL_Event>();
  auto keymap = registry.ctx<const keymap::Keymap>();

  for (auto&& cause : input_nodes) {
    auto event = std::get<SDL_Event>(cause->value());
    if (event.type != SDL_KEYDOWN) {
      continue;
    }
    if (auto input_action = keymap.get_action(event.key.keysym.sym); input_action) {
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
          cne.add_effect(cause, intent::Walk{player, vector.value()});
        }

        // Other input_action -> intent translations will go here
      }
    }
  }
}

void walk(entt::registry& registry) {
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

  registry.set<CauseAndEffect>();
  registry.set<keymap::Keymap>(keymap::main);
  registry.set<State>(false);

  while (!registry.ctx<const State>().quit) {  // Game loop.
    registry.ctx<CauseAndEffect&>().reset();
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
