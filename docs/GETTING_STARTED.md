# Getting Started with Lumina Engine

Welcome to Lumina Engine! This guide will help you set up your first project using the engine.

## Table of Contents

- [Prerequisites](#prerequisites)
- [Building the Engine](#building-the-engine)
- [Creating Your First Project](#creating-your-first-project)
- [Project Structure](#project-structure)
- [Running Examples](#running-examples)
- [Troubleshooting](#troubleshooting)
- [Next Steps](#next-steps)

## Prerequisites

Before you begin, ensure you have the following installed on your system:

| Requirement | Version | Notes |
|-------------|---------|-------|
| C++ Compiler | GCC 7+, Clang 5+, MSVC 2017+ | Must support C++17 |
| CMake | 3.14+ | Build system |
| SDL3 | Latest | Windowing and rendering |
| Lua | 5.4+ | Scripting |
| Box2D | Latest | Physics (optional) |

### Platform-Specific Setup

#### Linux (Ubuntu/Debian)

```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    libsdl3-dev \
    liblua5.4-dev \
    libbox2d-dev
```

#### Linux (Arch)

```bash
sudo pacman -S \
    base-devel \
    cmake \
    sdl3 \
    lua \
    box2d
```

#### macOS

```bash
brew install cmake sdl3 lua box2d
```

#### Windows

Using vcpkg:
```bash
vcpkg install sdl3:x64-windows lua:x64-windows box2d:x64-windows
```

## Building the Engine

### Step 1: Clone the Repository

```bash
git clone https://github.com/lumina-engine/lumina-engine.git
cd lumina-engine
```

### Step 2: Configure with CMake

```bash
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### Step 3: Build

```bash
make -j$(nproc)
```

### Step 4: Install (Optional)

```bash
sudo make install
```

Or use the CMake install command:
```bash
cmake --install . --prefix /usr/local
```

## Creating Your First Project

### 1. Create Project Directory

```bash
mkdir MyLuminaGame
cd MyLuminaGame
```

### 2. Create Project Structure

```
MyLuminaGame/
├── CMakeLists.txt
├── main.cpp
├── scripts/
│   └── main.lua
└── assets/
    └── (your game assets)
```

### 3. Create `main.cpp`

Create a file named `main.cpp`:

```cpp
#include <LuminaEngine/LuminaEngine.h>
#include <LuminaEngine/core/Engine.h>
#include <iostream>

int main() {
    Lumina::Engine engine;

    // Initialize with title, width, and height
    if (!engine.Initialize("My First Lumina Game", 1280, 720)) {
        std::cerr << "Failed to initialize Lumina Engine" << std::endl;
        return -1;
    }

    std::cout << "Lumina Engine initialized successfully!" << std::endl;

    // Load Lua script for game logic
    engine.LoadScript("scripts/main.lua");

    // Run the main game loop
    engine.Run();

    // Clean shutdown
    engine.Shutdown();

    return 0;
}
```

### 4. Create a Lua Script

Create `scripts/main.lua`:

```lua
-- Simple game script
Lumina.Log.Info("Hello from Lumina Engine!")

function OnUpdate(dt)
    -- Called every frame
    -- dt = delta time in seconds
    
    if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
        Lumina.Quit()
    end
end

function OnRender()
    -- Called every frame after update
end
```

### 5. Create `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyLuminaGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Lumina Engine (adjust path if not installed)
find_package(LuminaEngine REQUIRED)

add_executable(MyLuminaGame main.cpp)

target_link_libraries(MyLuminaGame PRIVATE LuminaEngine::LuminaEngine)

# Copy scripts to build directory
file(COPY scripts DESTINATION ${CMAKE_BINARY_DIR})
```

### 6. Build and Run

```bash
mkdir build
cd build
cmake ..
make
./MyLuminaGame
```

## Project Structure

A typical Lumina Engine project:

```
MyLuminaGame/
├── CMakeLists.txt
├── main.cpp
├── src/
│   └── (additional C++ source files)
├── scripts/
│   ├── main.lua
│   ├── entities.lua
│   └── config.lua
├── assets/
│   ├── textures/
│   │   └── player.png
│   └── audio/
│       └── jump.wav
└── data/
    └── savegame.json
```

## Running Examples

Lumina Engine includes example projects in the `examples/` directory:

```bash
# After building the engine
cd build
./bin/lumina_example

# List available examples
ls bin/
```

## Troubleshooting

### CMake Can't Find Lumina Engine

If you're developing locally without installing:

```cmake
# In your project's CMakeLists.txt
add_subdirectory(path/to/lumina-engine)
```

### Linker Errors

Ensure you're using the same C++ standard:

```cmake
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
```

### SDL3 Not Found

Install SDL3 development libraries or let CMake fetch them:

```bash
# CMake will automatically download SDL3 if not found
cmake .. -DFETCHCONTENT_UPDATES_DISCONNECTED=OFF
```

### Lua Script Not Loading

Check the working directory - paths are relative to where you run the executable:

```cpp
// Use absolute path for debugging
engine.LoadScript("/full/path/to/script.lua");
```

### Performance Issues

Enable optimizations in Release mode:

```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
```

### Getting Help

- Open an issue on [GitHub](https://github.com/lumina-engine/lumina-engine/issues)
- Check the [Scripting Guide](SCRIPTING_GUIDE.md)
- Review the API documentation

## Next Steps

- [Scripting Guide](SCRIPTING_GUIDE.md) - Learn how to use Lua with Lumina Engine
- [API Reference](API_REFERENCE.md) - Complete API documentation
- [Physics Guide](PHYSICS_GUIDE.md) - Using Box2D for physics
- Explore the `examples/` directory in the source code
