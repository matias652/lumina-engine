# Getting Started with Lumina Engine

Welcome to Lumina Engine! This guide will help you set up your first project using the engine.

## Prerequisites

Before you begin, ensure you have the following installed on your system:

- A C++ compiler supporting C++17 (GCC 7+, Clang 5+, or MSVC 2017+)
- CMake 3.14 or higher
- SDL3
- Lua 5.4 or higher
- (Optional) Box2D

## Building the Engine

Clone the repository and build it using CMake:

```bash
git clone https://github.com/lumina-engine/lumina-engine.git
cd lumina-engine
mkdir build
cd build
cmake ..
make
sudo make install
```

## Creating Your First Project

### 1. Project Structure

Create a new directory for your project:

```bash
mkdir MyLuminaGame
cd MyLuminaGame
```

### 2. Create `main.cpp`

Create a file named `main.cpp` with the following content:

```cpp
#include <LuminaEngine/LuminaEngine.h>
#include <LuminaEngine/core/Engine.h>
#include <iostream>

int main() {
    Lumina::Engine engine;

    if (!engine.Initialize("My First Lumina Game", 1280, 720)) {
        std::cerr << "Failed to initialize Lumina Engine" << std::endl;
        return -1;
    }

    std::cout << "Lumina Engine initialized successfully!" << std::endl;

    engine.Run();
    engine.Shutdown();

    return 0;
}
```

### 3. Create `CMakeLists.txt`

Create a `CMakeLists.txt` to build your project:

```cmake
cmake_minimum_required(VERSION 3.14)
project(MyLuminaGame)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(LuminaEngine REQUIRED)

add_executable(MyLuminaGame main.cpp)
target_link_libraries(MyLuminaGame PRIVATE LuminaEngine::LuminaEngine)
```

### 4. Build and Run

```bash
mkdir build
cd build
cmake ..
make
./MyLuminaGame
```

## Next Steps

- Check out the [Scripting Guide](SCRIPTING_GUIDE.md) to learn how to use Lua with Lumina Engine.
- Explore the API Reference (coming soon) for more details on available classes and functions.
