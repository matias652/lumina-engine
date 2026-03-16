#include "LuminaEngine/core/Engine.h"
#include "LuminaEngine/input/Input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "LuminaEngine/graphics/TextureManager.h"
#include "LuminaEngine/graphics/Graphics.h"
#include "LuminaEngine/physics/Physics.h"

#ifdef LUMINA_GLES_AVAILABLE
#include <GLES3/gl3.h>
#endif

#ifdef LUA_AVAILABLE
#include "LuminaEngine/scripting/ScriptEngine.h"
#endif

#include "LuminaEngine/utils/Logger.h"

namespace Lumina {

#ifdef LUMINA_GLES_AVAILABLE
static SDL_GLContext s_glContext = nullptr;
#endif

Engine::Engine() 
    : m_isRunning(false)
    , m_initialized(false)
    , m_width(1280)
    , m_height(720)
    , m_windowTitle("Lumina Engine")
    , m_window(nullptr)
    , m_renderer(nullptr)
    , m_physicsWorld(std::make_unique<PhysicsWorld>())
    , m_physicsAccumulator(0.0f)
    , m_lastFrameTime(0)
    , m_startTime(0)
    , m_deltaTime(0.0f)
    , m_fps(0.0f)
    , m_frameCount(0)
    , m_fpsTimer(0.0f)
{
    #ifdef LUA_AVAILABLE
    m_scriptEngine = std::make_unique<ScriptEngine>();
    #endif
}

Engine::~Engine() {
    Shutdown();
}

bool Engine::Initialize(const std::string& windowTitle, int width, int height) {
    m_windowTitle = windowTitle;
    m_width = width;
    m_height = height;

    // Enable logging at Info level by default so engine messages are visible.
    // Users can call Logger::Initialize() again before this to set a custom level.
    if (!Logger::IsEnabled()) {
        Logger::Initialize(true, LogLevel::Info);
    }

    // Initialize SDL3
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        LUMINA_LOG_ERROR("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_WindowFlags windowFlags = SDL_WINDOW_RESIZABLE;

    #ifdef LUMINA_GLES_AVAILABLE
    // Configure OpenGL ES attributes before creating the window
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    windowFlags |= SDL_WINDOW_OPENGL;
    #endif

    // Create window
    m_window = SDL_CreateWindow(m_windowTitle.c_str(), m_width, m_height, windowFlags);
    if (!m_window) {
        LUMINA_LOG_ERROR("Failed to create window: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    #ifdef LUMINA_GLES_AVAILABLE
    s_glContext = SDL_GL_CreateContext(m_window);
    if (s_glContext) {
        SDL_GL_MakeCurrent(m_window, s_glContext);
        SDL_GL_SetSwapInterval(1); // VSync
        glEnable(GL_DEPTH_TEST);
        glViewport(0, 0, m_width, m_height);
    } else {
        LUMINA_LOG_WARNING("Failed to create GLES context: %s", SDL_GetError());
    }
    #endif

    // Create renderer
    m_renderer = SDL_CreateRenderer(m_window, nullptr);
    if (!m_renderer) {
        LUMINA_LOG_ERROR("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(m_window);
        SDL_Quit();
        return false;
    }

    // Initialize TextureManager
    TextureManager::Init(m_renderer);

    // Initialize Graphics system
    Graphics::Init(m_renderer);

    // Initialize physics world
    m_physicsWorld->Init(0.0f, -9.8f);

    // Initialize ScriptEngine (Lua/sol2)
    #ifdef LUA_AVAILABLE
    if (m_scriptEngine) {
        m_scriptEngine->Initialize(this);
    }
    #endif

    // Initialize Input system
    Input::Init();

    LUMINA_LOG_INFO("Engine initialized successfully.");
    m_lastFrameTime = SDL_GetPerformanceCounter();
    m_startTime = SDL_GetPerformanceCounter();
    m_isRunning = true;
    m_initialized = true;
    
    return true;
}

void Engine::Run() {
    if (!m_isRunning) {
        return;
    }
    
    LUMINA_LOG_INFO("Engine started running.");
    
    // Main Loop
    while (m_isRunning) {
        m_deltaTime = CalculateDeltaTime();
        
        Input::Update();
        ProcessEvents();
        Update(m_deltaTime);
        Render();
        
        SDL_RenderPresent(m_renderer);
    }
    
    LUMINA_LOG_INFO("Engine stopped.");
}

void Engine::ProcessEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        Input::ProcessEvent(event);
        
        switch (event.type) {
            case SDL_EVENT_QUIT:
            case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
                m_isRunning = false;
                break;
            case SDL_EVENT_KEY_DOWN:
                if (event.key.key == SDLK_ESCAPE) {
                    m_isRunning = false;
                }
                break;
            case SDL_EVENT_WINDOW_RESIZED:
                SDL_GetWindowSize(m_window, &m_width, &m_height);
                #ifdef LUMINA_GLES_AVAILABLE
                if (s_glContext) {
                    glViewport(0, 0, m_width, m_height);
                }
                #endif
                break;
            default:
                break;
        }
    }
}

void Engine::Update(float deltaTime) {
    // Update physics with a fixed timestep accumulator to ensure stable simulation.
    // Box2D requires a consistent step size; variable deltas cause tunnelling and jitter.
    constexpr float FIXED_STEP = 1.0f / 60.0f;
    m_physicsAccumulator += deltaTime;
    while (m_physicsAccumulator >= FIXED_STEP) {
        m_physicsWorld->Step(FIXED_STEP);
        m_physicsAccumulator -= FIXED_STEP;
    }
    
    // Update scripts
    #ifdef LUA_AVAILABLE
    if (m_scriptEngine) {
        m_scriptEngine->Update(deltaTime);
    }
    #endif
}

void Engine::Render() {
    // Clear previous frame
    SDL_SetRenderDrawColor(m_renderer, 20, 20, 30, 255);
    SDL_RenderClear(m_renderer);

    #ifdef LUMINA_GLES_AVAILABLE
    SDL_FlushRenderer(m_renderer); // Ensure SDL clear is processed first
    if (s_glContext) {
        glClear(GL_DEPTH_BUFFER_BIT);
    }
    #endif
    
    // Render scripts (Sprites)
    #ifdef LUA_AVAILABLE
    if (m_scriptEngine) {
        m_scriptEngine->Render();
    }
    #endif
}

float Engine::CalculateDeltaTime() {
    const uint64_t currentTime = SDL_GetPerformanceCounter();
    float deltaTime = static_cast<float>(currentTime - m_lastFrameTime) / static_cast<float>(SDL_GetPerformanceFrequency());
    m_lastFrameTime = currentTime;
    
    // Limit deltaTime to avoid large jumps
    if (deltaTime > 0.1f) {
        deltaTime = 0.1f;
    }
    
    // Calculate FPS
    m_frameCount++;
    m_fpsTimer += deltaTime;
    if (m_fpsTimer >= 1.0f) {
        m_fps = static_cast<float>(m_frameCount) / m_fpsTimer;
        m_frameCount = 0;
        m_fpsTimer = 0.0f;
    }
    
    return deltaTime;
}

void Engine::Shutdown() {
    if (!m_initialized) {
        return;
    }
    m_initialized = false;
    m_isRunning = false;

    #ifdef LUMINA_GLES_AVAILABLE
    if (s_glContext) {
        SDL_GL_DestroyContext(s_glContext);
        s_glContext = nullptr;
    }
    #endif

    // Clean up TextureManager
    TextureManager::Shutdown();

    // Clean up Graphics system
    Graphics::Shutdown();

    // Clean up ScriptEngine
    #ifdef LUA_AVAILABLE
    if (m_scriptEngine) {
        m_scriptEngine->Shutdown();
        m_scriptEngine.reset();
    }
    #endif

    // Clean up physics world
    if (m_physicsWorld) {
        m_physicsWorld->Shutdown();
        m_physicsWorld.reset();
    }
    
    // Clean up SDL3
    if (m_renderer) {
        SDL_DestroyRenderer(m_renderer);
        m_renderer = nullptr;
    }
    
    if (m_window) {
        SDL_DestroyWindow(m_window);
        m_window = nullptr;
    }
    
    SDL_Quit();
}

#ifdef LUA_AVAILABLE
bool Engine::LoadScript(const std::string& filename) {
    if (m_scriptEngine && m_scriptEngine->IsInitialized()) {
        return m_scriptEngine->LoadScript(filename);
    }
    return false;
}
#endif

// Window methods
void Engine::SetWindowTitle(const std::string& title) {
    m_windowTitle = title;
    if (m_window) {
        SDL_SetWindowTitle(m_window, title.c_str());
    }
}

std::string Engine::GetWindowTitle() const {
    return m_windowTitle;
}

void Engine::SetFullscreen(bool enabled) {
    if (m_window) {
        SDL_SetWindowFullscreen(m_window, enabled ? SDL_WINDOW_FULLSCREEN : 0);
    }
}

bool Engine::IsFullscreen() const {
    if (m_window) {
        return (SDL_GetWindowFlags(m_window) & SDL_WINDOW_FULLSCREEN) != 0;
    }
    return false;
}

void Engine::MinimizeWindow() {
    if (m_window) {
        SDL_MinimizeWindow(m_window);
    }
}

void Engine::MaximizeWindow() {
    if (m_window) {
        SDL_MaximizeWindow(m_window);
    }
}

// Time methods
float Engine::GetTime() const {
    if (m_startTime == 0) return 0.0f;
    uint64_t currentTime = SDL_GetPerformanceCounter();
    return static_cast<float>(currentTime - m_startTime) / static_cast<float>(SDL_GetPerformanceFrequency());
}

float Engine::GetDeltaTime() const {
    return m_deltaTime;
}

float Engine::GetFPS() const {
    return m_fps;
}

// Physics methods (forwarded to PhysicsWorld)
void Engine::SetGravity(float x, float y) {
    if (m_physicsWorld) {
        m_physicsWorld->SetGravity(x, y);
    }
}

std::pair<float, float> Engine::GetGravity() const {
    if (m_physicsWorld) {
        return m_physicsWorld->GetGravity();
    }
    return {0.0f, -9.8f};
}

int Engine::CreateBody(float x, float y, bool isDynamic) {
    if (m_physicsWorld) {
        return m_physicsWorld->CreateBody(x, y, isDynamic);
    }
    return -1;
}

void Engine::DestroyBody(int bodyId) {
    if (m_physicsWorld) {
        m_physicsWorld->DestroyBody(bodyId);
    }
}

void Engine::ApplyForce(int bodyId, float forceX, float forceY) {
    if (m_physicsWorld) {
        m_physicsWorld->ApplyForce(bodyId, forceX, forceY);
    }
}

void Engine::ApplyImpulse(int bodyId, float ix, float iy) {
    if (m_physicsWorld) {
        m_physicsWorld->ApplyImpulse(bodyId, ix, iy);
    }
}

std::pair<float, float> Engine::GetPosition(int bodyId) const {
    if (m_physicsWorld) {
        return m_physicsWorld->GetPosition(bodyId);
    }
    return {0.0f, 0.0f};
}

void Engine::SetPosition(int bodyId, float x, float y) {
    if (m_physicsWorld) {
        m_physicsWorld->SetPosition(bodyId, x, y);
    }
}

std::pair<float, float> Engine::GetVelocity(int bodyId) const {
    if (m_physicsWorld) {
        return m_physicsWorld->GetVelocity(bodyId);
    }
    return {0.0f, 0.0f};
}

void Engine::SetVelocity(int bodyId, float vx, float vy) {
    if (m_physicsWorld) {
        m_physicsWorld->SetVelocity(bodyId, vx, vy);
    }
}

} // namespace Lumina
