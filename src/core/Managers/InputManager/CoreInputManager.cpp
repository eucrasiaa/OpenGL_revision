#include "Managers/InputManager/CoreInputManager.hpp"
#include "Logger/ILogger.hpp"
#include <thread>


// FROM TEMPLATE
CoreInputManager::CoreInputManager(ILogger* logger) : logger_(logger) {

  if (!logger_) { throw std::runtime_error("RenderServer's windowServer cannot be null"); }
  for (auto& binding : scancodeToAction_) {
    binding.count = 0;

    std::fill(binding.actions, binding.actions + 3, UNBOUND);
    // binding.actions.fill(UNBOUND);
  }

  // Build reverse lookup: scancode -> multiple actions
  for (const auto& binding : ACTION_BINDINGS) {
    for (size_t i = 0; i < binding.count; ++i) {
      SDL_Scancode sc = binding.scancodes[i];
      if (sc == SDL_SCANCODE_UNKNOWN) continue;

      uint8_t actionId = static_cast<uint8_t>(binding.action);
      auto& scancodeBinding = scancodeToAction_[sc];

      // add action to this scancode's list (dup avoid)
      bool alreadyExists = false;
      for (size_t j = 0; j < scancodeBinding.count; ++j) {
        if (scancodeBinding.actions[j] == actionId) {
          alreadyExists = true;
          break;
        }
      }

      if (!alreadyExists && scancodeBinding.count < MAX_BINDINGS_PER_KEY) {
        scancodeBinding.actions[scancodeBinding.count++] = actionId;
      }
    }
  }
validateBindings();
}

inline bool CoreInputManager::isPressed(ActionID action) const {
  uint8_t state = actionStates_[static_cast<uint8_t>(action)];
  return (state == 0b01); // prev up, curr down
}
inline bool CoreInputManager::isHeld(ActionID action) const {
  return (actionStates_[static_cast<uint8_t>(action)] & 0b01) != 0;
}

inline bool CoreInputManager::isReleased(ActionID action) const {
  uint8_t state = actionStates_[static_cast<uint8_t>(action)];
  return (state == 0b10); // prev down, curr up
}
inline Vec2 CoreInputManager::getVector(ActionID neg_x,ActionID pos_x,ActionID neg_y,ActionID pos_y) const{
  float x = 0.0f, y = 0.0f;
  if (isHeld(neg_x))  x -= 1.0f;
  if (isHeld(pos_x)) x += 1.0f;
  if (isHeld(neg_y))    y -= 1.0f;
  if (isHeld(pos_y))  y += 1.0f;

  return Vec2(x,y).normalized();
}


//TODO redo this logic when we do joystick input stuff lol
inline float CoreInputManager::getAxis(ActionID neg_x,ActionID pos_x) const{
  float x = 0.0f;
  if (isHeld(neg_x))  x -= 1.0f;
  if (isHeld(pos_x)) x += 1.0f;
  return x;
}

inline Vec2 CoreInputManager::getMovementAxis() const {
  float x = 0.0f, y = 0.0f;
  if (isHeld(ActionID::INPUT_LEFT))  x -= 1.0f;
  if (isHeld(ActionID::INPUT_RIGHT)) x += 1.0f;
  if (isHeld(ActionID::INPUT_UP))    y -= 1.0f;
  if (isHeld(ActionID::INPUT_DOWN))  y += 1.0f;

  // Normalize if needed
 // return (mag > 1.0f) ? Vec2f{x/mag, y/mag} : Vec2f{x, y};
  return Vec2(x,y).normalized();
}

inline void CoreInputManager::updateState() {
  // std::cout<<debugState()<<std::flush;
  // std::this_thread::sleep_for(std::chrono::milliseconds(16));
  // term::move_up(static_cast<size_t>(ActionID::TOTAL_ACTION_COUNT)+1,true);
  // Shift all states: previous = current
for (uint8_t& state : actionStates_) {
    // Shift current bit (0) to previous bit (1), AND preserve current bit (0)
    state = ((state & 0b01) << 1) | (state & 0b01); 
  }
}



void CoreInputManager::processEvents(const SDL_Event& event) {
  if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
    SDL_Scancode code = event.key.keysym.scancode;
    const auto& scancodeBinding = scancodeToAction_[code];

    if (scancodeBinding.count > 0) {
      bool isDown = (event.type == SDL_KEYDOWN);
// ignore repeats for now
      if (!event.key.repeat) {
        // update all actions bound to this scancode
        for (size_t i = 0; i < scancodeBinding.count; ++i) {
          uint8_t actionId = scancodeBinding.actions[i];
          if (actionId != UNBOUND) {
            updateActionState(actionId, isDown);
          }
        }
      }
    }
  }
  // TODO: Handle mouse events similarly
}


// NON TEMPLATED 
void CoreInputManager::updateActionState(uint8_t actionId, bool isDown) {
  uint8_t& state = actionStates_[actionId];
  state = STATE_TRANSITIONS[isDown ? 1 : 0][state];

  // uint8_t& state = actionStates_[actionId];

  // Preserve previous state in bit1, set current state in bit0
  // hift existing current bit to previous, then set new current
  // uint8_t previousState = (state & 0b01) << 1; // Move current to previous position
  // uint8_t currentState = isDown ? 0b01 : 0;    // New current state
  // state = previousState | currentState;
}



bool CoreInputManager::validateBindings() {
  bool isValid = true;
    // check scancode -> multiple actions mapping
    std::array<ScancodeBinding, MAX_SCANCODES> tempLookup{};
    for (auto& binding : tempLookup) {
        binding.count = 0;
    std::fill(binding.actions, binding.actions + 3, UNBOUND);
        // binding.actions.fill(UNBOUND);
    }
    
    for (const auto& binding : ACTION_BINDINGS) {
        for (size_t i = 0; i < binding.count; ++i) {
            SDL_Scancode sc = binding.scancodes[i];
            if (sc == SDL_SCANCODE_UNKNOWN) continue;
            
            uint8_t actionId = static_cast<uint8_t>(binding.action);
            auto& scBinding = tempLookup[sc];
            
            // check if already exists
            bool exists = false;
            for (size_t j = 0; j < scBinding.count; ++j) {
                if (scBinding.actions[j] == actionId) exists = true;
            }
            
            if (!exists && scBinding.count < MAX_BINDINGS_PER_KEY) {
                scBinding.actions[scBinding.count++] = actionId;
            } else if (!exists) {
                logger_->log(LogLevel::Error, "Action " + std::to_string(i) + " has no bindings!");
                isValid=false;
            }
        }
    }
    
    for (size_t i = 0; i < MAX_SCANCODES; ++i) {
        const auto& scBinding = tempLookup[i];
        if (scBinding.count > 1) {
            std::string actions = "";
            for (size_t j = 0; j < scBinding.count; ++j) {
                actions += std::to_string(scBinding.actions[j]) + " ";
            }
            logger_->log(LogLevel::Info, "Scancode " + std::to_string(i) + " has multiple: " + actions);
        }
    }
    return isValid;
}
