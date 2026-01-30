#include "LuminaEngine/core/Engine.h"
#include "LuminaEngine/input/Input.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_render.h>
#include "LuminaEngine/graphics/TextureManager.h"

#ifdef LUMINA_GLES_AVAILABLE
#include <GLES3/gl3.h>
#endif

#ifdef LUA_AVAILABLE
#include "LuminaEngine/scripting/ScriptEngine.h"
#endif

#ifdef BOX2D_AVAILABLE
#include <box2d/box2d.h>
#endif

#include "LuminaEngine/utils/Logger.h"

namespace Lumina {

#ifdef LUMINA_GLES_AVAILABLE
static SDL_GLContext s_glContext = nullptr;
#endif

Engine::Engine() 
    : m_isRunning(false)
    , m_width(1280)
    , m_height(720)
    , m_windowTitle("Lumina Engine")
    , m_window(nullptr)
    , m_renderer(nullptr)
    , m_lastFrameTime(0)
    , m_deltaTime(0.0f)
{
    #ifdef BOX2D_AVAILABLE
    m_physicsWorld = nullptr;
    #endif
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

    // Initialize Box2D
    #ifdef BOX2D_AVAILABLE
    if (m_physicsWorld == nullptr) {
        b2Vec2 gravity(0.0f, -9.8f);
        m_physicsWorld = new b2World(gravity);
    }
    #endif

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
    m_isRunning = true;
    
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
    // Update physics
    #ifdef BOX2D_AVAILABLE
    if (m_physicsWorld) {
        b2World* world = static_cast<b2World*>(m_physicsWorld);
        constexpr int32 velocityIterations = 6;
        constexpr int32 positionIterations = 2;
        world->Step(deltaTime, velocityIterations, positionIterations);
    }
    #endif
    
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
    
    // Render a simple rectangle as a test
    const float rectX = static_cast<float>(m_width) / 2.0f - 50.0f;
    const float rectY = static_cast<float>(m_height) / 2.0f - 50.0f;
    SDL_FRect rect = {rectX, rectY, 100.0f, 100.0f};
    SDL_SetRenderDrawColor(m_renderer, 100, 150, 255, 255);
    SDL_RenderFillRect(m_renderer, &rect);

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
    
    return deltaTime;
}

void Engine::Shutdown() {
    if (!m_isRunning) {
        return;
    }

    #ifdef LUMINA_GLES_AVAILABLE
    if (s_glContext) {
        SDL_GL_DestroyContext(s_glContext);
        s_glContext = nullptr;
    }
    #endif

    // Clean up TextureManager
    TextureManager::Shutdown();

    // Clean up ScriptEngine
    #ifdef LUA_AVAILABLE
    if (m_scriptEngine) {
        m_scriptEngine->Shutdown();
        m_scriptEngine.reset();
    }
    #endif

    // Clean up Box2D
    #ifdef BOX2D_AVAILABLE
    if (m_physicsWorld) {
        delete static_cast<b2World*>(m_physicsWorld);
        m_physicsWorld = nullptr;
    }
    #endif
    
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
    m_isRunning = false;
}

#ifdef LUA_AVAILABLE
bool Engine::LoadScript(const std::string& filename) {
    if (m_scriptEngine && m_scriptEngine->IsInitialized()) {
        return m_scriptEngine->LoadScript(filename);
    }
    return false;
}
#endif

} // namespace Lumina
