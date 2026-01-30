#include "LuminaEngine/LuminaEngine.h"
#include <iostream>

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    
    std::cout << "========================================" << std::endl;
    std::cout << "    Lumina Engine v1.0.0" << std::endl;
    std::cout << "    SDL3 + Box2D + Lua/sol2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    Lumina::Engine engine;

    if (!engine.Initialize("Lumina Engine", 1280, 720)) {
        std::cerr << "Error: Could not initialize the engine" << std::endl;
        return -1;
    }

    engine.Run();
    engine.Shutdown();

    return 0;
}
