#include "keymap.h"

namespace context::keymap {

std::optional<const InputAction> Keymap::get_action(const SDL_Keycode& keycode) const {
  auto it = keymap_.find(keycode);
  if (it == keymap_.end()) {
    return std::nullopt;
  } else {
    return it->second;
  }
}

bool Keymap::is_action(const SDL_Keycode& keycode, const InputAction& action) const {
  auto opt_action = get_action(keycode);
  if (!opt_action) {
    return false;
  } else {
    return opt_action.value() == action;
  }
}
}  // namespace context::keymap
