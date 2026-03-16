#include "LuminaEngine/scripting/ScriptEngine.h"
#include "LuminaEngine/core/Engine.h"
#include "LuminaEngine/utils/Logger.h"
#include "LuminaEngine/input/Input.h"
#include "LuminaEngine/graphics/Sprite.h"
#include "LuminaEngine/graphics/Graphics.h"
#include "LuminaEngine/audio/Audio.h"
#include "LuminaEngine/physics/Physics.h"
#include "LuminaEngine/LuminaEngine.h"
#include <sol/sol.hpp>
#include <SDL3/SDL.h>

namespace Lumina {

// PIMPL implementation for cached Lua functions
struct ScriptEngine::Impl {
    sol::protected_function onUpdate;
    sol::protected_function onRender;
};

ScriptEngine::ScriptEngine() 
    : m_engine(nullptr)
    , m_initialized(false)
    , m_impl(std::make_unique<Impl>())
{
}

ScriptEngine::~ScriptEngine() {
    Shutdown();
}

void ScriptEngine::Initialize(Engine* engine) {
    if (m_initialized) return;

    m_engine = engine;
    m_lua = std::make_unique<sol::state>();
    m_impl = std::make_unique<Impl>();
    
    // Load base Lua libraries
    m_lua->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::string, sol::lib::table, sol::lib::os);

    RegisterBindings();

    m_initialized = true;
    LUMINA_LOG_INFO("ScriptEngine initialized with full bindings");
}

void ScriptEngine::Shutdown() {
    if (m_lua) {
        m_lua.reset();
    }
    // Reset impl to release cached Lua function references.
    // Do not reallocate — if Initialize() is called again it will create a fresh Impl.
    m_impl.reset();
    m_engine = nullptr;
    m_initialized = false;
}

bool ScriptEngine::LoadScript(const std::string& filename) {
    if (!m_initialized || !m_lua) return false;

    auto result = m_lua->script_file(filename);
    if (!result.valid()) {
        sol::error err = result;
        LUMINA_LOG_ERROR("Lua Error: %s", err.what());
        return false;
    }

    // Cache common functions
    m_impl->onUpdate = (*m_lua)["OnUpdate"];
    m_impl->onRender = (*m_lua)["OnRender"];

    LUMINA_LOG_INFO("Script loaded: %s", filename.c_str());
    return true;
}

void ScriptEngine::Update(float dt) {
    if (!m_initialized || !m_impl || !m_impl->onUpdate.valid()) return;

    auto result = m_impl->onUpdate(dt);
    if (!result.valid()) {
        sol::error err = result;
        LUMINA_LOG_ERROR("Lua Runtime Error: %s", err.what());
    }
}

void ScriptEngine::Render() {
    if (!m_initialized || !m_impl || !m_impl->onRender.valid()) return;

    auto result = m_impl->onRender();
    if (!result.valid()) {
        sol::error err = result;
        LUMINA_LOG_ERROR("Lua Render Error: %s", err.what());
    }
}

void ScriptEngine::RegisterBindings() {
    // Main 'Lumina' namespace
    auto lumina = (*m_lua)["Lumina"].get_or_create<sol::table>();

    // =========================================================================
    // Lumina.Version
    // =========================================================================
    auto version = lumina["Version"].get_or_create<sol::table>();
    version.set_function("get", []() {
        return std::string(
            std::to_string(VERSION_MAJOR) + "." +
            std::to_string(VERSION_MINOR) + "." +
            std::to_string(VERSION_PATCH)
        );
    });
    version.set_function("getMajor", []() { return VERSION_MAJOR; });
    version.set_function("getMinor", []() { return VERSION_MINOR; });
    version.set_function("getPatch", []() { return VERSION_PATCH; });
    lumina["VERSION_MAJOR"] = VERSION_MAJOR;
    lumina["VERSION_MINOR"] = VERSION_MINOR;
    lumina["VERSION_PATCH"] = VERSION_PATCH;

    // =========================================================================
    // Lumina.Application
    // =========================================================================
    auto application = lumina["Application"].get_or_create<sol::table>();
    application.set_function("quit", []() {
        SDL_Event event;
        event.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&event);
    });
    application.set_function("isRunning", [this]() {
        return m_engine && m_engine->IsRunning();
    });
    application.set_function("loadScript", [this](const std::string& filename) {
        if (m_engine && m_engine->IsRunning()) {
            return m_engine->LoadScript(filename);
        }
        return false;
    });

    // =========================================================================
    // Lumina.Time
    // =========================================================================
    auto time = lumina["Time"].get_or_create<sol::table>();
    time.set_function("get", [this]() {
        return m_engine ? m_engine->GetTime() : 0.0f;
    });
    time.set_function("getDelta", [this]() {
        return m_engine ? m_engine->GetDeltaTime() : 0.0f;
    });
    time.set_function("getFPS", [this]() {
        return m_engine ? m_engine->GetFPS() : 0.0f;
    });

    // =========================================================================
    // Lumina.Window
    // =========================================================================
    auto window = lumina["Window"].get_or_create<sol::table>();
    window.set_function("getSize", [this]() {
        return std::make_tuple(m_engine->GetWidth(), m_engine->GetHeight());
    });
    window.set_function("setTitle", [this](const std::string& title) {
        if (m_engine) m_engine->SetWindowTitle(title);
    });
    window.set_function("getTitle", [this]() {
        return m_engine ? m_engine->GetWindowTitle() : std::string("");
    });
    window.set_function("setFullscreen", [this](bool enabled) {
        if (m_engine) m_engine->SetFullscreen(enabled);
    });
    window.set_function("isFullscreen", [this]() {
        return m_engine && m_engine->IsFullscreen();
    });
    window.set_function("minimize", [this]() {
        if (m_engine) m_engine->MinimizeWindow();
    });
    window.set_function("maximize", [this]() {
        if (m_engine) m_engine->MaximizeWindow();
    });

    // =========================================================================
    // Lumina.Input
    // =========================================================================
    auto input = lumina["Input"].get_or_create<sol::table>();
    input.set_function("getKey", &Input::GetKey);
    input.set_function("getKeyDown", &Input::GetKeyDown);
    input.set_function("getKeyUp", &Input::GetKeyUp);
    input.set_function("getMouseButton", &Input::GetMouseButton);
    input.set_function("getMouseButtonDown", &Input::GetMouseButtonDown);
    input.set_function("getMouseButtonUp", &Input::GetMouseButtonUp);
    input.set_function("getMousePosition", [](sol::this_state) {
        float x, y;
        Input::GetMousePosition(&x, &y);
        return std::make_tuple(x, y);
    });
    input.set_function("getMouseX", &Input::GetMouseX);
    input.set_function("getMouseY", &Input::GetMouseY);

    // =========================================================================
    // Lumina.Graphics
    // =========================================================================
    auto graphics = lumina["Graphics"].get_or_create<sol::table>();
    
    // Clear and Present
    graphics.set_function("clear", [](uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        Graphics::Clear(r, g, b, a);
    });
    graphics.set_function("present", []() {
        Graphics::Present();
    });
    
    // Primitives
    graphics.set_function("drawPoint", [](float x, float y, uint32_t color) {
        Graphics::DrawPoint(x, y, color);
    });
    graphics.set_function("drawLine", [](float x1, float y1, float x2, float y2, uint32_t color) {
        Graphics::DrawLine(x1, y1, x2, y2, color);
    });
    graphics.set_function("drawRect", [](float x, float y, float w, float h, uint32_t color) {
        Graphics::DrawRect(x, y, w, h, color);
    });
    graphics.set_function("drawRectFilled", [](float x, float y, float w, float h, uint32_t color) {
        Graphics::DrawRectFilled(x, y, w, h, color);
    });
    graphics.set_function("drawCircle", [](float x, float y, float radius, uint32_t color) {
        Graphics::DrawCircle(x, y, radius, color);
    });
    graphics.set_function("drawCircleFilled", [](float x, float y, float radius, uint32_t color) {
        Graphics::DrawCircleFilled(x, y, radius, color);
    });
    graphics.set_function("drawTriangle", [](float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {
        Graphics::DrawTriangle(x1, y1, x2, y2, x3, y3, color);
    });
    graphics.set_function("drawTriangleFilled", [](float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {
        Graphics::DrawTriangleFilled(x1, y1, x2, y2, x3, y3, color);
    });
    
    // Color utilities
    graphics.set_function("color", [](uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
        return Graphics::Color(r, g, b, a);
    });
    graphics.set_function("colorRGB", [](uint8_t r, uint8_t g, uint8_t b) {
        return Graphics::ColorRGB(r, g, b);
    });
    graphics.set_function("getColorR", &Graphics::GetColorR);
    graphics.set_function("getColorG", &Graphics::GetColorG);
    graphics.set_function("getColorB", &Graphics::GetColorB);
    graphics.set_function("getColorA", &Graphics::GetColorA);
    
    // Blend modes
    graphics.set_function("setBlendMode", &Graphics::SetBlendMode);
    graphics.set_function("getBlendMode", &Graphics::GetBlendMode);
    graphics.set_function("setAlpha", &Graphics::SetAlpha);
    graphics.set_function("getAlpha", &Graphics::GetAlpha);
    
    // Camera/Viewport
    graphics.set_function("setCamera", [](float x, float y) {
        Graphics::SetCamera(x, y);
    });
    graphics.set_function("getCamera", [](sol::this_state) {
        auto [x, y] = Graphics::GetCamera();
        return std::make_tuple(x, y);
    });
    graphics.set_function("setZoom", &Graphics::SetZoom);
    graphics.set_function("getZoom", &Graphics::GetZoom);
    graphics.set_function("setViewport", [](int x, int y, int w, int h) {
        Graphics::SetViewport(x, y, w, h);
    });
    graphics.set_function("getViewport", [](sol::this_state) {
        auto [x, y, w, h] = Graphics::GetViewport();
        return std::make_tuple(x, y, w, h);
    });
    
    // Texture management
    graphics.set_function("loadTexture", [](const std::string& filename) {
        return Graphics::LoadTexture(filename);
    });
    graphics.set_function("unloadTexture", &Graphics::UnloadTexture);
    graphics.set_function("drawTexture", [](int id, float x, float y) {
        Graphics::DrawTexture(id, x, y);
    });
    graphics.set_function("drawTextureEx", [](int id, float x, float y, double angle, float scaleX, float scaleY, bool flipH, bool flipV) {
        Graphics::DrawTextureEx(id, x, y, angle, scaleX, scaleY, flipH, flipV);
    });
    graphics.set_function("getTextureSize", [](int id) {
        auto [w, h] = Graphics::GetTextureSize(id);
        return std::make_tuple(w, h);
    });
    graphics.set_function("getTextureWidth", &Graphics::GetTextureWidth);
    graphics.set_function("getTextureHeight", &Graphics::GetTextureHeight);
    
    // Sprite creation
    graphics.set_function("createSprite", [](const std::string& filename) {
        return Graphics::CreateSprite(filename);
    });
    
    // Debug
    graphics.set_function("drawGrid", [](float spacing, uint32_t color) {
        Graphics::DrawGrid(spacing, color);
    });
    graphics.set_function("drawAxis", [](uint32_t colorX, uint32_t colorY) {
        Graphics::DrawAxis(colorX, colorY);
    });

    // Sprite usertype
    graphics.new_usertype<Sprite>("Sprite",
        sol::constructors<Sprite(const std::string&)>(),
        "SetPosition", &Sprite::SetPosition,
        "SetScale", &Sprite::SetScale,
        "SetRotation", &Sprite::SetRotation,
        "SetRect", &Sprite::SetRect,
        "Draw", &Sprite::Draw
    );

    // =========================================================================
    // Lumina.Audio
    // =========================================================================
    auto audio = lumina["Audio"].get_or_create<sol::table>();
    audio.set_function("loadSound", [](const char* filename) {
        return AudioManager::LoadSound(filename);
    });
    audio.set_function("playSound", [](int id, float volume) {
        AudioManager::PlaySound(id, volume);
    });
    audio.set_function("loadMusic", [](const char* filename) {
        return AudioManager::LoadMusic(filename);
    });
    audio.set_function("playMusic", [](int id, bool loop) {
        AudioManager::PlayMusic(id, loop);
    });
    audio.set_function("stopMusic", []() {
        AudioManager::StopMusic();
    });
    audio.set_function("setMusicVolume", &AudioManager::SetMusicVolume);
    audio.set_function("setMasterVolume", &AudioManager::SetMasterVolume);

    // =========================================================================
    // Lumina.Physics
    // =========================================================================
    auto physics = lumina["Physics"].get_or_create<sol::table>();
    physics.set_function("setGravity", [](float x, float y) {
        if (m_engine) m_engine->SetGravity(x, y);
    });
    physics.set_function("getGravity", [](sol::this_state) {
        if (m_engine) {
            return m_engine->GetGravity();
        }
        return std::make_tuple(0.0f, -9.8f);
    });
    physics.set_function("createBody", [](float x, float y, bool isDynamic) {
        if (m_engine) return m_engine->CreateBody(x, y, isDynamic);
        return -1;
    });
    physics.set_function("destroyBody", [](int bodyId) {
        if (m_engine) m_engine->DestroyBody(bodyId);
    });
    physics.set_function("applyForce", [](int bodyId, float fx, float fy) {
        if (m_engine) m_engine->ApplyForce(bodyId, fx, fy);
    });
    physics.set_function("applyImpulse", [](int bodyId, float ix, float iy) {
        if (m_engine) m_engine->ApplyImpulse(bodyId, ix, iy);
    });
    physics.set_function("getPosition", [](int bodyId) {
        if (m_engine) return m_engine->GetPosition(bodyId);
        return std::make_tuple(0.0f, 0.0f);
    });
    physics.set_function("setPosition", [](int bodyId, float x, float y) {
        if (m_engine) m_engine->SetPosition(bodyId, x, y);
    });
    physics.set_function("getVelocity", [](int bodyId) {
        if (m_engine) return m_engine->GetVelocity(bodyId);
        return std::make_tuple(0.0f, 0.0f);
    });
    physics.set_function("setVelocity", [](int bodyId, float vx, float vy) {
        if (m_engine) m_engine->SetVelocity(bodyId, vx, vy);
    });

    // =========================================================================
    // Lumina.Key (Constants)
    // =========================================================================
    auto key = lumina["Key"].get_or_create<sol::table>();
    key["Space"] = SDL_SCANCODE_SPACE;
    key["Escape"] = SDL_SCANCODE_ESCAPE;
    key["W"] = SDL_SCANCODE_W;
    key["A"] = SDL_SCANCODE_A;
    key["S"] = SDL_SCANCODE_S;
    key["D"] = SDL_SCANCODE_D;
    key["Up"] = SDL_SCANCODE_UP;
    key["Down"] = SDL_SCANCODE_DOWN;
    key["Left"] = SDL_SCANCODE_LEFT;
    key["Right"] = SDL_SCANCODE_RIGHT;
    key["Enter"] = SDL_SCANCODE_RETURN;
    key["LShift"] = SDL_SCANCODE_LSHIFT;
    key["RShift"] = SDL_SCANCODE_RSHIFT;
    key["LCtrl"] = SDL_SCANCODE_LCTRL;
    key["Alpha0"] = SDL_SCANCODE_0;
    key["Alpha1"] = SDL_SCANCODE_1;
    key["Alpha2"] = SDL_SCANCODE_2;
    key["Alpha3"] = SDL_SCANCODE_3;
    key["Alpha4"] = SDL_SCANCODE_4;
    key["Alpha5"] = SDL_SCANCODE_5;
    key["Alpha6"] = SDL_SCANCODE_6;
    key["Alpha7"] = SDL_SCANCODE_7;
    key["Alpha8"] = SDL_SCANCODE_8;
    key["Alpha9"] = SDL_SCANCODE_9;

    // =========================================================================
    // Lumina.Mouse (Constants)
    // =========================================================================
    auto mouse = lumina["Mouse"].get_or_create<sol::table>();
    mouse["Left"] = SDL_BUTTON_LEFT;
    mouse["Right"] = SDL_BUTTON_RIGHT;
    mouse["Middle"] = SDL_BUTTON_MIDDLE;
    mouse["X1"] = SDL_BUTTON_X1;
    mouse["X2"] = SDL_BUTTON_X2;
}

}