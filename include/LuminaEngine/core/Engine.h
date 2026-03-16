#pragma once

#include <string>
#include <memory>
#include <cstdint>

// Forward declarations
struct SDL_Window;
struct SDL_Renderer;

#ifdef LUA_AVAILABLE
namespace Lumina {
    class ScriptEngine;
}
#endif

namespace Lumina {

class PhysicsWorld;

class Engine {
public:
    Engine();
    ~Engine();

    // Initialize the engine
    bool Initialize(const std::string& windowTitle = "Lumina Engine", 
                    int width = 1280, int height = 720);
    
    // Run the main engine loop
    void Run();
    
    // Shutdown and clean up resources
    void Shutdown();

    // Getters
    bool IsRunning() const noexcept { return m_isRunning; }
    int GetWidth() const noexcept { return m_width; }
    int GetHeight() const noexcept { return m_height; }

#ifdef LUA_AVAILABLE
    // Load Lua script
    bool LoadScript(const std::string& filename);
#endif

private:
    bool m_isRunning;
    bool m_initialized;
    int m_width;
    int m_height;
    std::string m_windowTitle;
    
    // SDL3
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;
    
    // Physics world (Box2D wrapper)
    std::unique_ptr<PhysicsWorld> m_physicsWorld;

    // Physics fixed-timestep accumulator
    float m_physicsAccumulator;
    
    // Scripting (Lua/sol2)
    #ifdef LUA_AVAILABLE
    std::unique_ptr<ScriptEngine> m_scriptEngine;
    #endif
    
    // Time management
    uint64_t m_lastFrameTime;
    float m_deltaTime;
    
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    
    float CalculateDeltaTime();
};

} // namespace Lumina
