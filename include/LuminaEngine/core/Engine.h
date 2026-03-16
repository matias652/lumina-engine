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

    // Window methods
    void SetWindowTitle(const std::string& title);
    std::string GetWindowTitle() const;
    void SetFullscreen(bool enabled);
    bool IsFullscreen() const;
    void MinimizeWindow();
    void MaximizeWindow();

    // Time methods
    float GetTime() const;
    float GetDeltaTime() const;
    float GetFPS() const;

    // Physics methods (forwarded to PhysicsWorld)
    void SetGravity(float x, float y);
    std::pair<float, float> GetGravity() const;
    int CreateBody(float x, float y, bool isDynamic);
    void DestroyBody(int bodyId);
    void ApplyForce(int bodyId, float forceX, float forceY);
    void ApplyImpulse(int bodyId, float ix, float iy);
    std::pair<float, float> GetPosition(int bodyId) const;
    void SetPosition(int bodyId, float x, float y);
    std::pair<float, float> GetVelocity(int bodyId) const;
    void SetVelocity(int bodyId, float vx, float vy);

    // Renderer access
    SDL_Renderer* GetRenderer() const { return m_renderer; }
    
    // Set window size (for game resolution changes)
    void SetWindowSize(int width, int height);

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
    uint64_t m_startTime;
    float m_deltaTime;
    float m_fps;
    int m_frameCount;
    float m_fpsTimer;
    
    void ProcessEvents();
    void Update(float deltaTime);
    void Render();
    
    float CalculateDeltaTime();
};

} // namespace Lumina
