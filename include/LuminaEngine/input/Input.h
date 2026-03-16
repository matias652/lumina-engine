#pragma once

#include <SDL3/SDL.h>
#include <array>

namespace Lumina {

class Input {
public:
    static void Init();
    static void Update(); // Call at the beginning of each frame
    static void ProcessEvent(const SDL_Event& event); // Call in the event loop

    // Keyboard
    static bool GetKey(SDL_Scancode key) noexcept;           // Held down
    static bool GetKeyDown(SDL_Scancode key) noexcept;       // Pressed in this frame
    static bool GetKeyUp(SDL_Scancode key) noexcept;         // Released in this frame

    // Mouse
    static bool GetMouseButton(Uint8 button) noexcept;
    static bool GetMouseButtonDown(Uint8 button) noexcept;
    static bool GetMouseButtonUp(Uint8 button) noexcept;
    static void GetMousePosition(float* x, float* y) noexcept;
    static float GetMouseX() noexcept;
    static float GetMouseY() noexcept;

private:
    static const bool* s_keyboardState;
    static std::array<bool, SDL_SCANCODE_COUNT> s_prevKeyboardState;
    static Uint32 s_mouseState;
    static Uint32 s_prevMouseState;
    static float s_mouseX, s_mouseY;
};

} // namespace Lumina