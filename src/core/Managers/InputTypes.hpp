#pragma once
#include <SDL_scancode.h>
#include <array>


static constexpr size_t MAX_ACTIONS = 64;
static constexpr size_t MAX_SCANCODES = SDL_NUM_SCANCODES; // ~ 300 ish 
static constexpr size_t MAX_BINDINGS_PER_KEY = 3;
static constexpr uint8_t UNBOUND = 255;

struct ScancodeBinding {
    uint8_t actions[MAX_BINDINGS_PER_KEY];
    uint8_t count;
};
struct MouseState {
  int x = 0, y = 0;
  int dx = 0, dy = 0;
  int wheelX = 0, wheelY = 0;
  uint8_t buttonStates[5] = {0};
};


static constexpr uint8_t STATE_TRANSITIONS[2][4] = {
    // isDown = false
    {
        0b00, // IDLE -> IDLE
        0b10, // PRESSED (0b01) -> RELEASED (0b10)
        0b00, // RELEASED (0b10) -> IDLE (0b00) 
        0b10  // HELD (0b11) -> RELEASED (0b10)
    },
    // isDown = true
    {
        0b01, // IDLE -> PRESSED
        0b11, // PRESSED -> HELD
        0b01, // RELEASED -> PRESSED
        0b11  // HELD -> HELD
    }
};

enum class ActionState : uint8_t {
    IDLE     = 0b0000,  // not pressed
    PRESSED  = 0b0001,  // just pressed this frame
    HELD     = 0b0011,  // pressed last frame AND this frame
    RELEASED = 0b0010,  // just released this frame
    //tbd
    REPEATED = 0b0111   // held with repeat (for text input)
};

enum class ActionID : uint8_t {
  // arrow keys + WASD
  INPUT_UP,
  INPUT_DOWN,
  INPUT_LEFT,
  INPUT_RIGHT,
  // arrow keys + ENTER, SPACE, X
  UI_UP,
  UI_DOWN,
  UI_LEFT,
  UI_RIGHT,  
  UI_ACCEPT, // ENTER, SPACE 
  UI_CANCEL, // X 
             //MISC
  KEY_E,
  KEY_Q,
  KEY_Z,
  KEY_X,
  KEY_C,
  KEY_V,
  // dev handy
  QUIT_GAME, //~
             // ... more actions
  TOTAL_ACTION_COUNT  // Keep as last
};


static_assert(static_cast<size_t>(ActionID::TOTAL_ACTION_COUNT) <= MAX_ACTIONS, 
              "Too many actions defined");
// compile-time mapping from action to scancodes
struct ActionBinding {
  ActionID action;
  std::array<SDL_Scancode, 2> scancodes; // max 2 keys per. if absolutly need, can change
  uint8_t count;
};


// gen at compile time!
//constexpr std::array<ActionBinding, 7> ACTION_BINDINGS = {{
constexpr std::array<ActionBinding, static_cast<size_t>(ActionID::TOTAL_ACTION_COUNT)> ACTION_BINDINGS = {{
  {ActionID::INPUT_UP,    {SDL_SCANCODE_W, SDL_SCANCODE_UP}, 2},
  {ActionID::INPUT_DOWN,  {SDL_SCANCODE_S, SDL_SCANCODE_DOWN}, 2},
  {ActionID::INPUT_LEFT,  {SDL_SCANCODE_A, SDL_SCANCODE_LEFT}, 2},
  {ActionID::INPUT_RIGHT, {SDL_SCANCODE_D, SDL_SCANCODE_RIGHT}, 2},
  {ActionID::UI_UP,       {SDL_SCANCODE_UP, SDL_SCANCODE_UNKNOWN}, 1},
  {ActionID::UI_DOWN,     {SDL_SCANCODE_DOWN, SDL_SCANCODE_UNKNOWN}, 1},
  {ActionID::UI_LEFT,     {SDL_SCANCODE_LEFT, SDL_SCANCODE_UNKNOWN}, 1},
  {ActionID::UI_RIGHT,    {SDL_SCANCODE_RIGHT, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::UI_ACCEPT,   {SDL_SCANCODE_SPACE, SDL_SCANCODE_RETURN}, 2}, 
    {ActionID::UI_CANCEL,   {SDL_SCANCODE_ESCAPE, SDL_SCANCODE_X}, 2}, 
    {ActionID::KEY_E,       {SDL_SCANCODE_E, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::KEY_Q,       {SDL_SCANCODE_Q, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::KEY_Z,       {SDL_SCANCODE_Z, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::KEY_X,       {SDL_SCANCODE_X, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::KEY_C,       {SDL_SCANCODE_C, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::KEY_V,       {SDL_SCANCODE_V, SDL_SCANCODE_UNKNOWN}, 1},
    {ActionID::QUIT_GAME,   {SDL_SCANCODE_ESCAPE, SDL_SCANCODE_UNKNOWN}, 1},
}};


// // tbd
//
// constexpr std::string_view trim(std::string_view str) {
//     while (!str.empty() && (str.front() == ' ' || str.front() == '\t' || str.front() == '\r')) str.remove_prefix(1);
//     while (!str.empty() && (str.back() == ' ' || str.back() == '\t' || str.back() == '\r')) str.remove_suffix(1);
//     return str;
// }
//
// constexpr std::string_view BINDINGS = R"(
//     input_up:    W, UP
//     input_down:  S, DOWN
//     input_left:  A, LEFT
//     input_right: D, RIGHT
//     quit_game:   ESC
//     resize_down: [
//     resize_up:   ]
// )";
