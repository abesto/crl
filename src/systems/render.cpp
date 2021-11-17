#include "render.h"

namespace systems::render {

void map(entt::registry& registry) {
  auto& map = registry.ctx<const context::map::Map&>();
  auto& console = registry.ctx<tcod::Console>();
  for (int x = 0; x < map.width(); ++x) {
    for (int y = 0; y < map.height(); ++y) {
      auto& map_tile = map.at(x, y);
      auto& console_tile = console.at(x, y);
      console_tile.ch = map_tile.glyph;
    }
  }
}

void renderables(entt::registry& registry) {
  auto& console = registry.ctx<tcod::Console>();
  for (auto&& [entity, renderable, position] : registry.view<Renderable, Coord>().each()) {
    auto& tile = console.at(position.x, position.y);
    tile.ch = renderable.glyph;
    tile.fg = renderable.color;
  }
}

void update(entt::registry& registry) {
  auto& console = registry.ctx<tcod::Console>();
  console.clear();

  map(registry);
  renderables(registry);

  auto& context = registry.ctx<tcod::ContextPtr>();
  context->present(console);
}

}  // namespace systems::render
