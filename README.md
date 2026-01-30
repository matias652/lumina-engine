# Lumina Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![CI](https://github.com/lumina-engine/lumina-engine/actions/workflows/ci.yml/badge.svg)](https://github.com/lumina-engine/lumina-engine/actions/workflows/ci.yml)
[![C++ Standard](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)

A C++ game engine built from scratch with SDL3, Box2D, and Lua scripting (sol2).

> [!IMPORTANT]
> **Beta Version**: Lumina Engine is currently in its beta stage. I am fully aware that there is still a long way to go before it becomes a complete, production-ready framework. Your patience and feedback are greatly appreciated!

## 🌟 Join the Journey

I'm passionate about building this engine using **Antigravity**, and I'd love to have you involved!
- **Contribute**: I would be absolutely thrilled if you could contribute in any way—whether it's fixing bugs, adding features, or improving documentation.
- **Follow the development**: I will soon be launching a **YouTube channel** and an **Instagram account** where I'll be documenting the entire development process, sharing devlogs, and showing how the engine evolves. Stay tuned!

## Features

- 🎮 **SDL3**: High-performance windowing, rendering, and event system
- 🎲 **Box2D**: 2D physics engine (optional)
- 📜 **Lua/sol2**: Integrated scripting system
- 🧩 **EnTT**: Fast and modern Entity Component System (ECS)
- 📋 **JSON**: Serialization and configuration with nlohmann/json
- 🚀 **Performance**: Optimized for high efficiency
- 📦 **Framework**: Designed as a reusable library
- 🔧 **CMake**: Modern and flexible build system

## Requirements

- **C++ Compiler**: GCC 7.0+, Clang 5.0+, or MSVC 2017+ (requires C++17)
- **CMake**: Version 3.14 or higher
- **SDL3**: Low-level library for graphics and audio
- **Lua**: Version 5.4+ (for scripting)
- **Box2D**: 2D physics engine (optional)
- **EnTT**: ECS framework (header-only)
- **nlohmann/json**: JSON library (header-only)

### Installing Dependencies

#### Arch Linux
```bash
sudo pacman -S sdl3 lua box2d cmake
```

#### Ubuntu/Debian
```bash
sudo apt-get install libsdl3-dev liblua5.4-dev libbox2d-dev cmake
```

#### Fedora
```bash
sudo dnf install SDL3-devel lua-devel box2d-devel cmake
```

**Note**: Most dependencies (sol2, EnTT, nlohmann/json) are automatically downloaded during compilation using CMake's FetchContent if they are not found on your system.

## Compilation

### Basic Compilation

```bash
mkdir build
cd build
cmake ..
make
```

### Build Options

```bash
# Release Mode (optimized)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Debug Mode
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build as shared library
cmake -DLUMINA_BUILD_SHARED_LIBS=ON ..

# Disable examples
cmake -DLUMINA_BUILD_EXAMPLES=OFF ..
```

### Installation

```bash
cd build
cmake --install . --prefix /usr/local
```

## Using as a Framework

### CMake (recommended)

In your `CMakeLists.txt`:

```cmake
find_package(LuminaEngine REQUIRED)

target_link_libraries(your_game PRIVATE LuminaEngine::LuminaEngine)
```

### C++ Code

```cpp
#include "LuminaEngine/LuminaEngine.h"

int main() {
    Lumina::Engine engine;
    
    if (!engine.Initialize("My Game", 1280, 720)) {
        return -1;
    }
    
    engine.Run();
    engine.Shutdown();
    
    return 0;
}
```

### Lua Scripts

```lua
-- script.lua
local version = Lumina.getVersion()
print("Engine version: " .. version)
```

## Project Structure

```
lumina-engine/
├── assets/
│   └── logo.svg                    # Project logo
├── include/
│   └── LuminaEngine/
│       ├── LuminaEngine.h          # Main header
│       ├── core/
│       │   └── Engine.h            # Main engine class
│       ├── graphics/
│       │   ├── Sprite.h            # Sprite rendering
│       │   └── TextureManager.h    # Texture management
│       ├── input/
│       │   └── Input.h             # Input handling
│       ├── scripting/
│       │   └── ScriptEngine.h      # Scripting system (Lua/sol2)
│       └── utils/
│           └── Logger.h            # Logging system
├── src/
│   ├── core/
│   │   └── Engine.cpp              # Engine implementation
│   ├── graphics/
│   │   ├── Sprite.cpp
│   │   └── TextureManager.cpp
│   ├── input/
│   │   └── Input.cpp
│   ├── scripting/
│   │   └── ScriptEngine.cpp
│   └── utils/
│       └── Logger.cpp
├── scripts/
│   └── example.lua                 # Lua script example
├── flatpak/                        # Flatpak distribution files
├── CMakeLists.txt                  # CMake configuration
├── LICENSE                         # MIT License
├── README.md                       # This file
└── .gitignore
```

## Documentation

Comprehensive documentation is available in the `docs/` directory:

- [Getting Started](docs/GETTING_STARTED.md)
- [Scripting Guide](docs/SCRIPTING_GUIDE.md)
- [Lua API Reference](lua_api_reference.lua)

## Main API

### C++ Interface

```cpp
#include "LuminaEngine/LuminaEngine.h"

int main() {
    Lumina::Engine engine;
    if (engine.Initialize("My Game")) {
        engine.LoadScript("scripts/main.lua");
        engine.Run();
    }
    return 0;
}
```

### Lua Interface

```lua
function OnUpdate(dt)
    if Lumina.Input.GetKeyDown(Lumina.Key.Space) then
        Lumina.Log.Info("Space pressed!")
    end
end
```

## Optimizations

The framework includes several performance optimizations:

- Compilation with `-O3 -march=native` in Release mode
- Use of forward declarations to reduce dependencies
- Efficient memory management with smart pointers
- Limited DeltaTime to avoid large jumps
- VSync enabled for smooth rendering
- No logging by default (zero overhead)

## Project Status

### Completed ✅

- ✅ Basic engine structure
- ✅ Initialization system
- ✅ Main loop with SDL3
- ✅ Event system (window, keyboard)
- ✅ Basic rendering with SDL3
- ✅ Scripting system with Lua/sol2
- ✅ Sprite rendering system
- ✅ Optional Box2D integration
- ✅ Time management and deltaTime
- ✅ Logging system (silent by default)
- ✅ Flatpak/Flathub configuration
- ✅ Input system (Keyboard & Mouse)
- ✅ Texture management

### Pending ⏳

- ⏳ Audio system (WIP)
- ⏳ Resource management system (for models/fonts)
- ⏳ Scene system
- ⏳ Entity Component System (EnTT integrated, but not fully exposed)
- ⏳ Unit tests
- ⏳ Advanced shaders support

## 🚀 Roadmap: Next Update

I'm actively working on the following improvements for the next release:

- ⚡ **Performance Boosts**: Further optimizations in the rendering pipeline and memory management.
- 🎮 **Input Overhaul**: Better support for gamepads and customizable key bindings.
- 🎨 **Enhanced Graphics**: New functions for particle systems, post-processing effects, and advanced sprite blending.
- ⌨️ **Input Power**: More robust event handling and mouse gesture support.

## Dependencies

- **SDL3**: Windowing and rendering system
- **Box2D**: 2D physics engine (optional)
- **Lua 5.4+**: Scripting language
- **sol2**: C++ bindings for Lua (header-only)

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Contributions

Contributions are welcome. Please:

1. Fork the repository
2. Create a branch for your feature (`git checkout -b feature/new-feature`)
3. Commit your changes (`git commit -am 'Add new feature'`)
4. Push to the branch (`git push origin feature/new-feature`)
5. Open a Pull Request

## Contact

For questions or suggestions, please open an issue in the repository.

---

Made with ❤️ by the Lumina Engine community
