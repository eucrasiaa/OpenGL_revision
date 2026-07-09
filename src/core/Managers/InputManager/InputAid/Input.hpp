#pragma once

#include "InputManager/IInputManager.hpp"
class Input {
  private:  
    friend class IEngine;
    friend class IWindowServer;
    friend struct wEngApplication;

    static inline const IInputManager* sInstance_ = nullptr;

  public:
    static inline bool isPressed(ActionID action){
        return sInstance_ ? sInstance_->isPressed(action) : false;
    }

    static inline bool isHeld(ActionID action){
      return sInstance_ ? sInstance_->isHeld(action) : false;
    }

    static inline bool isReleased(ActionID action){
        return sInstance_ ? sInstance_->isReleased(action) : false;

    }

    static inline Vec2 getVector(ActionID neg_x,ActionID pos_x,ActionID neg_y,ActionID pos_y){
        return sInstance_ ? sInstance_->getVector(neg_x,pos_x,neg_y,pos_y) : Vec2(0.0f);

    }

    static inline float getAxis(ActionID neg_x,ActionID pos_x){
        return sInstance_ ? sInstance_->getAxis(neg_x, pos_x) : 0.0f;

    }
    static inline Vec2 getMovementAxis(){
        return sInstance_ ? sInstance_->getMovementAxis() : Vec2(0.0f);

    }
};
