# API Reference

Complete API reference for Lumina Engine.

## Table of Contents

- [Engine](#engine)
- [Input](#input)
- [Graphics](#graphics)
  - [Sprite](#sprite)
  - [TextureManager](#texturemanager)
- [Physics](#physics)
- [Audio](#audio)
- [Scripting](#scripting)
- [Logging](#logging)

---

## Engine

Main engine class that manages the game loop, window, and subsystems.

### Header

```cpp
#include <LuminaEngine/core/Engine.h>
```

### Class Definition

```cpp
namespace Lumina {
    class Engine {
    public:
        Engine();
        ~Engine();
        
        bool Initialize(const std::string& windowTitle = "Lumina Engine", 
                        int width = 1280, int height = 720);
        void Run();
        void Shutdown();
        
        bool IsRunning() const noexcept;
        int GetWidth() const noexcept;
        int GetHeight() const noexcept;
        
        bool LoadScript(const std::string& filename);
    };
}
```

### Methods

| Method | Description |
|--------|-------------|
| `Initialize(title, width, height)` | Initialize the engine with window settings |
| `Run()` | Start the main game loop |
| `Shutdown()` | Clean up and release all resources |
| `IsRunning()` | Returns true if the engine is running |
| `GetWidth()` | Returns window width in pixels |
| `GetHeight()` | Returns window height in pixels |
| `LoadScript(filename)` | Load a Lua script file |

### Usage Example

```cpp
#include <LuminaEngine/core/Engine.h>
#include <iostream>

int main() {
    Lumina::Engine engine;
    
    if (!engine.Initialize("My Game", 1280, 720)) {
        std::cerr << "Failed to initialize engine" << std::endl;
        return 1;
    }
    
    // Load game scripts
    engine.LoadScript("scripts/game.lua");
    
    // Run the main loop
    engine.Run();
    
    // Clean shutdown
    engine.Shutdown();
    
    return 0;
}
```

---

## Input

Handles keyboard and mouse input.

### Header

```cpp
#include <LuminaEngine/input/Input.h>
```

### Initialization

```cpp
// Call once at startup
Lumina::Input::Init();

// Call at the beginning of each frame
Lumina::Input::Update();

// Call for each SDL event
Lumina::Input::ProcessEvent(event);
```

### Keyboard Methods

| Method | Description |
|--------|-------------|
| `GetKey(key)` | Returns true while key is held down |
| `GetKeyDown(key)` | Returns true on the frame key was pressed |
| `GetKeyUp(key)` | Returns true on the frame key was released |

### Mouse Methods

| Method | Description |
|--------|-------------|
| `GetMouseButton(button)` | Returns true while mouse button held |
| `GetMouseButtonDown(button)` | Returns true on frame button pressed |
| `GetMouseButtonUp(button)` | Returns true on frame button released |
| `GetMousePosition(x, y)` | Get current mouse position |

### Key Codes

Use SDL scancodes (e.g., `SDL_SCANCODE_W`, `SDL_SCANCODE_SPACE`):

```cpp
#include <SDL3/SDL.h>

if (Lumina::Input::GetKey(SDL_SCANCODE_W)) {
    // Move forward
}
```

### Mouse Buttons

| Constant | Button |
|----------|--------|
| `SDL_BUTTON_LEFT` | Left button |
| `SDL_BUTTON_RIGHT` | Right button |
| `SDL_BUTTON_MIDDLE` | Middle button |

---

## Graphics

### Sprite

2D image rendering with position, scale, and rotation.

#### Header

```cpp
#include <LuminaEngine/graphics/Sprite.h>
```

#### Constructor

```cpp
Sprite(const std::string& filename);  // Load from file
```

#### Methods

| Method | Description |
|--------|-------------|
| `SetPosition(x, y)` | Set sprite position |
| `SetScale(x, y)` | Set sprite scale (1.0 = original size) |
| `SetRotation(angle)` | Set rotation in degrees |
| `SetRect(x, y, w, h)` | Set source rectangle for animations |
| `Draw()` | Render the sprite |

#### Usage Example

```cpp
#include <LuminaEngine/graphics/Sprite.h>

// Create a sprite
Lumina::Sprite player("assets/player.png");

// Set properties
player.SetPosition(100.0f, 200.0f);
player.SetScale(2.0f, 2.0f);
player.SetRotation(45.0);

// In render loop
player.Draw();
```

---

### TextureManager

Manages texture loading and caching.

#### Header

```cpp
#include <LuminaEngine/graphics/TextureManager.h>
```

#### Methods

| Method | Description |
|--------|-------------|
| `LoadTexture(id, filename)` | Load texture and store with ID |
| `GetTexture(id)` | Retrieve texture by ID |
| `RemoveTexture(id)` | Unload texture from memory |
| `Clear()` | Unload all textures |

#### Usage Example

```cpp
Lumina::TextureManager& texMgr = Lumina::TextureManager::GetInstance();

// Load textures
texMgr.LoadTexture("player", "assets/player.png");
texMgr.LoadTexture("enemy", "assets/enemy.png");

// Get texture
SDL_Texture* tex = texMgr.GetTexture("player");
```

---

## Physics

Box2D physics integration for 2D physics simulation.

### Header

```cpp
#include <LuminaEngine/physics/Physics.h>
```

> **Note**: Requires `LUMINA_ENABLE_PHYSICS=ON` at compile time.

### PhysicsWorld

```cpp
class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();
    
    void Init(float gravityX = 0.0f, float gravityY = -9.8f);
    void Shutdown();
    
    void Step(float deltaTime);
    void SetGravity(float x, float y);
    
    // Body management
    int CreateBody(float x, float y, bool isDynamic = true);
    void DestroyBody(int bodyId);
    
    // Forces
    void ApplyForce(int bodyId, float forceX, float forceY);
    void ApplyImpulse(int bodyId, float impulseX, float impulseY);
    
    // State
    void GetPosition(int bodyId, float* outX, float* outY);
    void SetPosition(int bodyId, float x, float y);
    void GetVelocity(int bodyId, float* outVx, float* outVy);
    void SetVelocity(int bodyId, float vx, float vy);
};
```

### Usage Example

```cpp
#include <LuminaEngine/physics/Physics.h>

Lumina::PhysicsWorld physics;

// Initialize with gravity (m/s²)
physics.Init(0.0f, -9.8f);

// Create a dynamic body
int ballId = physics.CreateBody(0.0f, 5.0f, true);

// In update loop
physics.Step(deltaTime);

// Apply forces
physics.ApplyForce(ballId, 10.0f, 0.0f);

// Get position
float x, y;
physics.GetPosition(ballId, &x, &y);

// Clean up
physics.Shutdown();
```

---

## Audio

Audio playback system (WIP).

### Header

```cpp
#include <LuminaEngine/audio/Audio.h>
```

### Methods

| Method | Description |
|--------|-------------|
| `Initialize()` | Initialize audio system |
| `LoadMusic(filename)` | Load background music |
| `PlayMusic()` | Start playing music |
| `StopMusic()` | Stop music |
| `LoadSound(filename)` | Load sound effect |
| `PlaySound(id)` | Play sound effect |

---

## Scripting

Lua scripting integration via sol2.

### Header

```cpp
#include <LuminaEngine/scripting/ScriptEngine.h>
```

### ScriptEngine

```cpp
class ScriptEngine {
public:
    ScriptEngine();
    ~ScriptEngine();
    
    void Initialize(Engine* engine);
    void Shutdown();
    
    bool LoadScript(const std::string& filename);
    void Update(float dt);
    void Render();
};
```

### Lua API

See [Scripting Guide](SCRIPTING_GUIDE.md) for complete Lua API.

---

## Logging

Logging system with multiple severity levels.

### Header

```cpp
#include <LuminaEngine/utils/Logger.h>
```

### Initialization

```cpp
// Enable logging with level
Lumina::Logger::Initialize(true, Lumina::LogLevel::Debug);
```

### Log Levels

```cpp
enum class LogLevel {
    None = 0,    // No logging
    Error = 1,   // Errors only
    Warning = 2, // Warnings and errors
    Info = 3,    // Info, warnings, errors
    Debug = 4    // Everything
};
```

### Methods

| Method | Description |
|--------|-------------|
| `Initialize(enabled, level)` | Initialize the logger |
| `SetEnabled(bool)` | Enable/disable logging |
| `SetLevel(level)` | Set minimum log level |
| `Error(msg)` | Log error message |
| `Warning(msg)` | Log warning |
| `Info(msg)` | Log info message |
| `Debug(msg)` | Log debug message |

### Usage Example

```cpp
#include <LuminaEngine/utils/Logger.h>

// Enable logging
Lumina::Logger::Initialize(true, Lumina::LogLevel::Debug);

// Log messages
Lumina::Logger::Info("Application started");
Lumina::Logger::Warning("Low memory");
Lumina::Logger::Error("Failed to load: %s", filename);
Lumina::Logger::Debug("Player position: %f, %f", x, y);
```

### Format Strings

The logger supports printf-style formatting:

```cpp
Lumina::Logger::Info("Value: %d, Name: %s", value, name);
Lumina::Logger::Debug("Position: (%.2f, %.2f)", x, y);
```

---

## Build Configuration

### CMake Options

| Option | Description | Default |
|--------|-------------|---------|
| `CMAKE_BUILD_TYPE` | Build type | Release |
| `LUMINA_BUILD_SHARED_LIBS` | Build as DLL | OFF |
| `LUMINA_BUILD_EXAMPLES` | Build examples | ON |
| `LUMINA_ENABLE_PHYSICS` | Enable Box2D | ON |
| `LUMINA_ENABLE_LOGGING` | Enable logging | OFF |

### Feature Flags

| Macro | Description |
|-------|-------------|
| `LUA_AVAILABLE` | Lua scripting enabled |
| `BOX2D_AVAILABLE` | Physics enabled |
| `LUMINA_LOGGING_ENABLED` | Logging macros enabled |
