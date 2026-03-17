#include "LuminaEngine/LuminaEngine.h"
#include <iostream>
#include <string>
#include <cstring>

static void print_usage(const char* program) {
    std::cout << "Usage: " << program << " [options] [script.lua]" << std::endl;
    std::cout << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  -t, --title <title>    Window title (default: \"Lumina Engine\")" << std::endl;
    std::cout << "  -w, --width <width>    Window width (default: 1280)" << std::endl;
    std::cout << "  -h, --height <height>  Window height (default: 720)" << std::endl;
    std::cout << "  -v, --version          Show version and exit" << std::endl;
    std::cout << "  --help                 Show this help message" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "  " << program << " scripts/main.lua" << std::endl;
    std::cout << "  " << program << " -t \"My Game\" -w 1920 -h 1080 game.lua" << std::endl;
}

int main(int argc, char* argv[]) {
    std::string windowTitle = "Lumina Engine";
    int width = 1280;
    int height = 720;
    std::string scriptFile;

    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];

        if (arg == "--help") {
            print_usage(argv[0]);
            return 0;
        } else if (arg == "-v" || arg == "--version") {
            std::cout << "Lumina Engine v1.0.0" << std::endl;
            std::cout << "C++17 | SDL3 | Box2D | Lua/sol2 | EnTT" << std::endl;
            return 0;
        } else if ((arg == "-t" || arg == "--title") && i + 1 < argc) {
            windowTitle = argv[++i];
        } else if ((arg == "-w" || arg == "--width") && i + 1 < argc) {
            width = std::atoi(argv[++i]);
            if (width <= 0) width = 1280;
        } else if ((arg == "-h" || arg == "--height") && i + 1 < argc) {
            height = std::atoi(argv[++i]);
            if (height <= 0) height = 720;
        } else if (arg[0] != '-') {
            scriptFile = arg;
        } else {
            std::cerr << "Unknown option: " << arg << std::endl;
            print_usage(argv[0]);
            return 1;
        }
    }

    std::cout << "========================================" << std::endl;
    std::cout << "    Lumina Engine v1.0.0" << std::endl;
    std::cout << "    SDL3 + Box2D + Lua/sol2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;

    if (!scriptFile.empty()) {
        std::cout << "Script: " << scriptFile << std::endl;
    }
    std::cout << "Window: " << width << "x" << height << std::endl;
    std::cout << std::endl;

    Lumina::Engine engine;

    if (!engine.Initialize(windowTitle, width, height)) {
        std::cerr << "Error: Could not initialize the engine" << std::endl;
        return -1;
    }

    // Load script if specified
#ifdef LUA_AVAILABLE
    if (!scriptFile.empty()) {
        if (!engine.LoadScript(scriptFile)) {
            std::cerr << "Warning: Could not load script: " << scriptFile << std::endl;
        }
    }
#else
    if (!scriptFile.empty()) {
        std::cerr << "Warning: Lua scripting not available (compiled without LUA_AVAILABLE)" << std::endl;
    }
#endif

    engine.Run();
    engine.Shutdown();

    return 0;
}
