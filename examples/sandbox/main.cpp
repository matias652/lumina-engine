#include "LuminaEngine/LuminaEngine.h"
#include <iostream>

int main(int argc, char* argv[]) {
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

#ifdef LUA_AVAILABLE
    // Load script passed as first argument (e.g. from `lumina run myscript.lua`)
    if (argc > 1) {
        if (!engine.LoadScript(argv[1])) {
            std::cerr << "Warning: Failed to load script: " << argv[1] << std::endl;
        }
    }
#else
    (void)argc;
    (void)argv;
#endif

    engine.Run();
    // Shutdown is handled by ~Engine() via RAII; no manual call needed.

    return 0;
}
