#include "render.h"

namespace systems::render {

void update(entt::registry& registry) {
  auto& console = registry.ctx<tcod::Console>();
  auto& context = registry.ctx<tcod::ContextPtr>();

  console.clear();
  for (auto&& [entity, renderable, position] : registry.view<Renderable, Coord>().each()) {
    auto& tile = console.at(position.x, position.y);
    tile.ch = renderable.glyph;
    tile.fg = renderable.color;
  }
  context->present(console);
}

}  // namespace systems::render
