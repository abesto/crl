#pragma once

#include <SDL2/SDL.h>

#include <initializer_list>
#include <map>
#include <optional>

#include "cne/input_action.h"

namespace keymap {

class Keymap {
 private:
  std::map<SDL_Keycode, InputAction> keymap_;

 public:
  Keymap(std::initializer_list<std::pair<const SDL_Keycode, InputAction>> keymap) : keymap_{keymap} {};
  std::optional<const InputAction> get_action(const SDL_Keycode& keycode) const;
  bool is_action(const SDL_Keycode& keycode, const InputAction& action) const;
};

static const Keymap main{
    {SDLK_ESCAPE, InputAction::Quit},
    {SDLK_LEFT, InputAction::West},
    {SDLK_RIGHT, InputAction::East},
    {SDLK_UP, InputAction::North},
    {SDLK_DOWN, InputAction::South},
};

}  // namespace keymap
