#include <SDL2/SDL.h>
#include <libtcod.h>

#include <entt/entt.hpp>
#include <optional>
#include <variant>

#include "cne.h"
#include "components.h"
#include "context.h"
#include "systems.h"

#define DISPATCH(system) systems::system::update(registry)

const int WIDTH = 80;
const int HEIGHT = 25;

int main(int argc, char* argv[]) {
  entt::registry registry;
  registry.set<tcod::Console>(WIDTH, HEIGHT);

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
  registry.emplace<Renderable>(player, '@', tcod::ColorRGB{255, 255, 255});

  registry.set<CauseAndEffect>();
  registry.set<context::keymap::Keymap>(context::keymap::main);
  registry.set<context::state::State>(false);
  registry.set<context::map::Map>(WIDTH, HEIGHT);

  auto& map = registry.ctx<context::map::Map>();
  map.at(30, 22) = context::map::WALL;
  map.at(41, 22) = context::map::WALL;

  while (!registry.ctx<const context::state::State&>().quit) {  // Game loop.
    registry.ctx<CauseAndEffect&>().reset();
    DISPATCH(render);
    DISPATCH(read_input);
    if (!registry.ctx<const CauseAndEffect&>().find_by_type<SDL_Event>().empty()) {
      DISPATCH(handle_input);
      DISPATCH(walk);
      DISPATCH(quit);
      DISPATCH(debug);
    }
  }

  return 0;
}
