#pragma once
#include <string>
#include <memory>

// Forward declarations - DO NOT include sol/forward.hpp here
// as it causes incomplete type issues with sol::protected_function
namespace sol { class state; }
namespace Lumina { class Engine; }

namespace Lumina {

class ScriptEngine {
public:
    ScriptEngine();
    ~ScriptEngine();

    void Initialize(Engine* engine);
    void Shutdown();
    bool LoadScript(const std::string& filename);
    void Update(float dt);
    void Render();
    bool IsInitialized() const noexcept { return m_initialized; }

private:
    void RegisterBindings();

    Engine* m_engine;
    std::unique_ptr<sol::state> m_lua;
    bool m_initialized;

    // Implementation details moved to .cpp file (PIMPL pattern for cached functions)
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

}