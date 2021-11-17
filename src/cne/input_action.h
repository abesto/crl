#pragma once

#include <ostream>

enum class InputAction { Quit, East, West, North, South, NorthEast, SouthEast, SouthWest, NorthWest };

std::ostream& operator<<(std::ostream& os, const InputAction& action);
