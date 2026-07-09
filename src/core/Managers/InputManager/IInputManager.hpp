#pragma once
#include "Managers/InputTypes.hpp"
#include "Vec2.hpp"
#include <SDL_events.h>
class IInputManager{
  private:
    friend class IEngine;
    friend class IWindowServer;
    
  public:
    virtual ~IInputManager() = default;
    virtual inline bool isPressed(ActionID action) const=0;
    virtual inline bool isHeld(ActionID action) const=0;

    virtual inline bool isReleased(ActionID action) const=0;

    // virtual inline Vec2 getVector(uint8_t neg_x,uint8_t pos_x,uint8_t neg_y,uint8_t pos_y)=0;

    virtual inline Vec2 getVector(ActionID neg_x,ActionID pos_x,ActionID neg_y,ActionID pos_y) const =0;
 
    virtual inline float getAxis(ActionID neg_x,ActionID pos_x) const =0;
    virtual inline Vec2 getMovementAxis() const=0;
    // perhaps inline. but im sure compiler would automatically
    // the thing called w/i while loop!!!
    virtual void processEvents(const SDL_Event& event)=0;
  
    //called before to resync curr -> prev
    virtual inline void updateState() =0;
};
