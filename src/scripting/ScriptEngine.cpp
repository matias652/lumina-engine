#include "LuminaEngine/scripting/ScriptEngine.h"
#include "LuminaEngine/core/Engine.h"
#include "LuminaEngine/utils/Logger.h"
#include "LuminaEngine/input/Input.h"
#include "LuminaEngine/graphics/Sprite.h"
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

    // --- Logger Bindings ---
    auto log = lumina["Log"].get_or_create<sol::table>();
    log.set_function("Info", [](const std::string& msg) { Logger::Info(msg); });
    log.set_function("Warning", [](const std::string& msg) { Logger::Warning(msg); });
    log.set_function("Error", [](const std::string& msg) { Logger::Error(msg); });
    log.set_function("Debug", [](const std::string& msg) { Logger::Debug(msg); });

    // --- Input Bindings ---
    auto input = lumina["Input"].get_or_create<sol::table>();
    input.set_function("GetKey", &Input::GetKey);
    input.set_function("GetKeyDown", &Input::GetKeyDown);
    input.set_function("GetKeyUp", &Input::GetKeyUp);
    input.set_function("GetMouseButton", &Input::GetMouseButton);
    input.set_function("GetMouseButtonDown", &Input::GetMouseButtonDown);
    input.set_function("GetMouseButtonUp", &Input::GetMouseButtonUp);
    // Helper to return x, y directly to Lua
    input.set_function("GetMousePosition", [](sol::this_state) {
        float x, y;
        Input::GetMousePosition(&x, &y);
        return std::make_tuple(x, y);
    });
    // Direct helpers for X and Y
    input.set_function("GetMouseX", [](sol::this_state) {
        float x, y;
        Input::GetMousePosition(&x, &y);
        return x;
    });
    input.set_function("GetMouseY", [](sol::this_state) {
        float x, y;
        Input::GetMousePosition(&x, &y);
        return y;
    });

    // --- Key Constants (Common subset) ---
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
    
    // Numbers
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

    // --- Version ---
    lumina.set_function("getVersion", []() {
        return std::string(
            std::to_string(VERSION_MAJOR) + "." +
            std::to_string(VERSION_MINOR) + "." +
            std::to_string(VERSION_PATCH)
        );
    });
    lumina["VERSION_MAJOR"] = VERSION_MAJOR;
    lumina["VERSION_MINOR"] = VERSION_MINOR;
    lumina["VERSION_PATCH"] = VERSION_PATCH;

    // --- Engine Control ---
    lumina.set_function("Quit", []() {
        SDL_Event event;
        event.type = SDL_EVENT_QUIT;
        SDL_PushEvent(&event);
    });
    
    lumina.set_function("GetWindowSize", [this]() {
        return std::make_tuple(m_engine->GetWidth(), m_engine->GetHeight());
    });

    // --- Time ---
    lumina.set_function("GetTime", []() {
        return static_cast<double>(SDL_GetTicks()) / 1000.0;
    });

    // --- Graphics ---
    auto graphics = lumina["Graphics"].get_or_create<sol::table>();
    
    graphics.new_usertype<Sprite>("Sprite",
        sol::constructors<Sprite(const std::string&)>(),
        "SetPosition", &Sprite::SetPosition,
        "SetScale", &Sprite::SetScale,
        "SetRotation", &Sprite::SetRotation,
        "SetRect", &Sprite::SetRect,
        "Draw", &Sprite::Draw
    );
}

}