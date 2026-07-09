#pragma once
#include "Managers/InputManager/IInputManager.hpp"
#include <SDL_events.h>
#include <string>


#include "ansi_min.hpp"
class ILogger;
class CoreInputManager : public IInputManager {
  private: 
    // Reverse lookup: scancode -> action (static, compile-time)!
    std::array<ScancodeBinding, MAX_SCANCODES> scancodeToAction_{};
    // Action states (2 bits each)
    std::array<uint8_t, MAX_ACTIONS> actionStates_{};
    struct MouseState mouseState_{};

    bool isInitialized_ = false;
    void updateActionState(uint8_t actionId, bool isDown);

    ILogger* logger_ = nullptr;



    bool validateBindings();
// Optional: Print state for debugging
std::string debugState() const {
    std::string result = "Action States:\n";
    for (size_t i = 0; i < static_cast<size_t>(ActionID::TOTAL_ACTION_COUNT); ++i) {
        uint8_t state = actionStates_[i];
        const char* stateName = "IDLE";
        if (state == 0b01) stateName = "PRESSED";
        else if (state == 0b10) stateName = "RELEASED";
        else if (state == 0b11) stateName = "HELD";
        
        // result += term::mv::CLEAR_LINE;
        result += "\33[K  Action " + std::to_string(i) + ": " + stateName + "\n";
    }
    return result;
}
  public:
  //template
    CoreInputManager(ILogger* logger_);
    // constexpr CoreInputManager() : scancodeToAction_{} {
    //   scancodeToAction_.fill(UNBOUND);
    //   // build reverse lookup (scancode -> action)
    //   // FROM ACTION_BINDINGS RN!!
    //   for (const auto& binding : ACTION_BINDINGS) {
    //     for (size_t i = 0; i < binding.count; ++i) {
    //       uint8_t idx = static_cast<uint8_t>(binding.action);
    //       scancodeToAction_[binding.scancodes[i]] = idx;
    //     }
    //   }
    // }


    virtual inline bool isPressed(ActionID action) const;
    
    virtual inline bool isHeld(ActionID action) const;

    virtual inline bool isReleased(ActionID action) const;

    virtual inline Vec2 getVector(ActionID neg_x,ActionID pos_x,ActionID neg_y,ActionID pos_y) const;

    virtual inline float getAxis(ActionID neg_x,ActionID pos_x) const;
    virtual inline Vec2 getMovementAxis() const;
    // perhaps inline. but im sure compiler would automatically
    virtual void processEvents(const SDL_Event& event);
    virtual inline void updateState();

    //others 

};
