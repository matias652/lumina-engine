#include "LuminaEngine/input/Input.h"
#include <cstring>

namespace Lumina {

// Static member initialization
const bool* Input::s_keyboardState = nullptr;
std::array<bool, SDL_SCANCODE_COUNT> Input::s_prevKeyboardState;
Uint32 Input::s_mouseState = 0;
Uint32 Input::s_prevMouseState = 0;
float Input::s_mouseX = 0.0f;
float Input::s_mouseY = 0.0f;

void Input::Init() {
    // SDL3 handles the internal array, we just get the pointer
    int numKeys;
    s_keyboardState = SDL_GetKeyboardState(&numKeys);
    s_prevKeyboardState.fill(false);
}

void Input::Update() {
    // Update previous states before processing new frame events
    if (s_keyboardState) {
        std::memcpy(s_prevKeyboardState.data(), s_keyboardState, SDL_SCANCODE_COUNT * sizeof(bool));
    }
    
    s_prevMouseState = s_mouseState;
    s_mouseState = SDL_GetMouseState(&s_mouseX, &s_mouseY);
}

void Input::ProcessEvent(const SDL_Event& event) {
    // SDL_GetKeyboardState is automatically updated with SDL_PumpEvents,
    // which is called internally by SDL_PollEvent, so we don't need to
    // process individual key events here for continuous state.
    (void)event;
}

bool Input::GetKey(SDL_Scancode key) noexcept {
    if (!s_keyboardState) return false;
    return s_keyboardState[key];
}

bool Input::GetKeyDown(SDL_Scancode key) noexcept {
    if (!s_keyboardState) return false;
    return s_keyboardState[key] && !s_prevKeyboardState[key];
}

bool Input::GetKeyUp(SDL_Scancode key) noexcept {
    if (!s_keyboardState) return false;
    return !s_keyboardState[key] && s_prevKeyboardState[key];
}

bool Input::GetMouseButton(Uint8 button) noexcept {
    return (s_mouseState & SDL_BUTTON_MASK(button)) != 0;
}

bool Input::GetMouseButtonDown(Uint8 button) noexcept {
    bool isDown = (s_mouseState & SDL_BUTTON_MASK(button)) != 0;
    bool wasDown = (s_prevMouseState & SDL_BUTTON_MASK(button)) != 0;
    return isDown && !wasDown;
}

bool Input::GetMouseButtonUp(Uint8 button) noexcept {
    bool isDown = (s_mouseState & SDL_BUTTON_MASK(button)) != 0;
    bool wasDown = (s_prevMouseState & SDL_BUTTON_MASK(button)) != 0;
    return !isDown && wasDown;
}

void Input::GetMousePosition(float* x, float* y) noexcept {
    if (x) *x = s_mouseX;
    if (y) *y = s_mouseY;
}

} // namespace Lumina