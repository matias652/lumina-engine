# Lumina Engine

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Version](https://img.shields.io/badge/Version-1.0.0-blue.svg)](https://github.com/lumina-engine/lumina-engine/releases)
[![CI](https://github.com/lumina-engine/lumina-engine/actions/workflows/ci.yml/badge.svg)](https://github.com/lumina-engine/lumina-engine/actions/workflows/ci.yml)
[![C++ Standard](https://img.shields.io/badge/C++-17-blue.svg)](https://en.cppreference.com/w/cpp/17)
[![Platform](https://img.shields.io/badge/Platform-Linux%20%7C%20Windows%20%7C%20macOS-lightgrey.svg)](https://github.com/lumina-engine/lumina-engine)

A modern C++ game engine built from scratch with SDL3, Box2D, and Lua scripting (sol2).

> [!IMPORTANT]
> **Beta Version**: Lumina Engine is currently in its beta stage. I am fully aware that there is still a long way to go before it becomes a complete, production-ready framework. Your patience and feedback are greatly appreciated!

## Table of Contents

- [Features](#features)
- [Quick Start](#quick-start)
- [Requirements](#requirements)
- [Installation](#installation)
  - [Installing Dependencies](#installing-dependencies)
  - [Building the Engine](#building-the-engine)
  - [Build Options](#build-options)
- [Using as a Framework](#using-as-a-framework)
- [Project Structure](#project-structure)
- [Documentation](#documentation)
- [Main API](#main-api)
  - [C++ Interface](#c++-interface)
  - [Lua Interface](#lua-interface)
- [Optimizations](#optimizations)
- [Project Status](#project-status)
- [Roadmap](#roadmap)
- [Contributing](#contributing)
- [License](#license)

## Quick Start

```bash
# Clone the repository
git clone https://github.com/lumina-engine/lumina-engine.git
cd lumina-engine

# Build
mkdir build && cd build
cmake ..
make

# Run examples
./bin/lumina_example
```

## Features

| Category | Feature | Description |
|----------|---------|-------------|
| **Windowing** | SDL3 Integration | High-performance windowing, rendering, and event system |
| **Physics** | Box2D | 2D physics engine (optional, can be disabled) |
| **Scripting** | Lua/sol2 | Integrated scripting system for game logic |
| **ECS** | EnTT | Fast and modern Entity Component System |
| **Serialization** | JSON | Configuration and save data with nlohmann/json |
| **Graphics** | SDL3 Renderer | Hardware-accelerated 2D rendering |
| **Input** | Keyboard/Mouse | Complete input handling system |
| **Build** | CMake | Modern and flexible build system |

### Key Capabilities

- **Cross-platform**: Linux, Windows, macOS support
- **Zero-dependency core**: Most dependencies auto-downloaded via FetchContent
- **Performance optimized**: Release builds use `-O3 -march=native`
- **Header-only libraries**: EnTT, nlohmann/json included automatically
- **Framework design**: Built as a reusable library

## Requirements

### System Requirements

| Requirement | Minimum Version | Notes |
|-------------|-----------------|-------|
| **C++ Compiler** | GCC 7.0+, Clang 5.0+, or MSVC 2017+ | Must support C++17 |
| **CMake** | 3.14+ | Build system |
| **SDL3** | Latest | Graphics and windowing |
| **Lua** | 5.4+ | Scripting runtime |
| **Box2D** | Latest | Physics (optional) |

### Auto-downloaded Dependencies

These libraries are automatically downloaded during CMake configuration if not found on your system:
- **sol2** - Lua C++ bindings (header-only)
- **EnTT** - Entity Component System (header-only)
- **nlohmann/json** - JSON library (header-only)

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

#### macOS (Homebrew)

```bash
brew install sdl3 lua box2d cmake
```

#### Windows

Install via [vcpkg](https://github.com/microsoft/vcpkg) or download precompiled binaries:
```bash
vcpkg install sdl3:x64-windows lua:x64-windows box2d:x64-windows
```

> **Note**: Most dependencies (sol2, EnTT, nlohmann/json) are automatically downloaded during compilation using CMake's FetchContent if they are not found on your system.

## Compilation

### Basic Compilation

```bash
mkdir build
cd build
cmake ..
make
```

### Build Options

| Option | Description | Default |
|--------|-------------|---------|
| `CMAKE_BUILD_TYPE` | Build type: Debug, Release, RelWithDebInfo, MinSizeRel | Release |
| `LUMINA_BUILD_SHARED_LIBS` | Build as shared library | OFF |
| `LUMINA_BUILD_EXAMPLES` | Build example applications | ON |
| `LUMINA_ENABLE_PHYSICS` | Enable Box2D physics support | ON |
| `LUMINA_ENABLE_LOGGING` | Enable logging system | OFF |

#### Build Examples

```bash
# Release Mode (optimized, default)
cmake -DCMAKE_BUILD_TYPE=Release ..

# Debug Mode with symbols
cmake -DCMAKE_BUILD_TYPE=Debug ..

# Build as shared library
cmake -DLUMINA_BUILD_SHARED_LIBS=ON ..

# Disable examples
cmake -DLUMINA_BUILD_EXAMPLES=OFF ..

# Enable physics (default)
cmake -DLUMINA_ENABLE_PHYSICS=ON ..

# Enable logging for debugging
cmake -DLUMINA_ENABLE_LOGGING=ON -DCMAKE_BUILD_TYPE=Debug ..
```

### Installation

```bash
cd build
cmake --install . --prefix /usr/local
```

## Using as a Framework

### CMake Integration (Recommended)

#### Option 1: Installed Package

In your `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LuminaEngine REQUIRED)

add_executable(MyGame main.cpp)
target_link_libraries(MyGame PRIVATE LuminaEngine::LuminaEngine)
```

#### Option 2: Subdirectory

Add Lumina Engine as a subdirectory in your project:

```cmake
# In your game's CMakeLists.txt
add_subdirectory(lumina-engine)

add_executable(MyGame main.cpp)
target_link_libraries(MyGame PRIVATE LuminaEngine::LuminaEngine)
```

#### Option 3: vcpkg

```bash
vcpkg install lumina-engine
```

Then in CMake:
```cmake
find_package(LuminaEngine REQUIRED)
target_link_libraries(MyGame PRIVATE LuminaEngine::LuminaEngine)
```

### C++ Code Example

```cpp
#include <LuminaEngine/LuminaEngine.h>
#include <LuminaEngine/core/Engine.h>
#include <LuminaEngine/graphics/Sprite.h>
#include <LuminaEngine/input/Input.h>
#include <iostream>

int main() {
    Lumina::Engine engine;
    
    // Initialize with title and resolution
    if (!engine.Initialize("My Game", 1280, 720)) {
        std::cerr << "Failed to initialize engine!" << std::endl;
        return -1;
    }
    
    // Load a Lua script for game logic
    engine.LoadScript("scripts/main.lua");
    
    // Run the main game loop
    engine.Run();
    
    // Clean shutdown
    engine.Shutdown();
    
    return 0;
}
```

### Lua Scripts

```lua
-- scripts/main.lua
local version = Lumina.getVersion()
print("Lumina Engine version: " .. version)

function OnUpdate(dt)
    -- Game logic here
    if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
        Lumina.Quit()
    end
end

function OnRender()
    -- Custom rendering here
end
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

| Document | Description |
|----------|-------------|
| [Getting Started](docs/GETTING_STARTED.md) | Quick start guide for new developers |
| [Scripting Guide](docs/SCRIPTING_GUIDE.md) | Lua scripting API and examples |
| [API Reference](docs/API_REFERENCE.md) | Complete C++ API reference |
| [Physics Guide](docs/PHYSICS_GUIDE.md) | Box2D physics integration |
| [Flatpak Guide](docs/FLATPAK_GUIDE.md) | Building and distributing Flatpak |

## Distribution

### Flatpak

Lumina Engine is available as a Flatpak for easy Linux distribution:

```bash
# Install from Flathub (once published)
flatpak install flathub org.luminaengine.LuminaEngine
flatpak run org.luminaengine.LuminaEngine
```

For building from source, see [Flatpak Guide](docs/FLATPAK_GUIDE.md).

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
