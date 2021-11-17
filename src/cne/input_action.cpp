#include "input_action.h"

std::ostream& operator<<(std::ostream& os, const InputAction& action) {
  switch (action) {
    case InputAction::Quit:
      os << "Quit";
      break;
    case InputAction::East:
      os << "East";
      break;
    case InputAction::West:
      os << "West";
      break;
    case InputAction::North:
      os << "North";
      break;
    case InputAction::South:
      os << "South";
      break;
    case InputAction::NorthEast:
      os << "NorthEast";
      break;
    case InputAction::SouthEast:
      os << "SouthEast";
      break;
    case InputAction::SouthWest:
      os << "SouthWest";
      break;
    case InputAction::NorthWest:
      os << "NorthWest";
      break;
  }
  return os;
}
