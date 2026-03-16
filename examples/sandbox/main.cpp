#include "LuminaEngine/LuminaEngine.h"
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
#include <windows.h>
#include <shellapi.h>
#else
#include <unistd.h>
#endif

namespace fs = std::filesystem;

namespace Lumina {

// Game configuration structure
struct GameConfig {
    int width = 1280;
    int height = 720;
    std::string mainScript = "main.lua";
    std::string gameName = "Game";
};

// Parse game config from JSON content
GameConfig ParseGameConfig(const std::string& jsonContent) {
    GameConfig config;
    
    // Simple JSON parsing (without external library)
    size_t widthPos = jsonContent.find("\"width\"");
    if (widthPos != std::string::npos) {
        size_t colonPos = jsonContent.find(":", widthPos);
        if (colonPos != std::string::npos) {
            config.width = std::stoi(jsonContent.substr(colonPos + 1));
        }
    }
    
    size_t heightPos = jsonContent.find("\"height\"");
    if (heightPos != std::string::npos) {
        size_t colonPos = jsonContent.find(":", heightPos);
        if (colonPos != std::string::npos) {
            config.height = std::stoi(jsonContent.substr(colonPos + 1));
        }
    }
    
    size_t scriptPos = jsonContent.find("\"mainScript\"");
    if (scriptPos != std::string::npos) {
        size_t colonPos = jsonContent.find(":", scriptPos);
        size_t quoteStart = jsonContent.find("\"", colonPos + 1);
        size_t quoteEnd = jsonContent.find("\"", quoteStart + 1);
        if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
            config.mainScript = jsonContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        }
    }
    
    size_t namePos = jsonContent.find("\"name\"");
    if (namePos != std::string::npos) {
        size_t colonPos = jsonContent.find(":", namePos);
        size_t quoteStart = jsonContent.find("\"", colonPos + 1);
        size_t quoteEnd = jsonContent.find("\"", quoteStart + 1);
        if (quoteStart != std::string::npos && quoteEnd != std::string::npos) {
            config.gameName = jsonContent.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
        }
    }
    
    return config;
}

// Extract zip file (using system unzip command)
bool ExtractZip(const std::string& zipPath, const std::string& destPath) {
    std::string command = "unzip -o -q \"" + zipPath + "\" -d \"" + destPath + "\" 2>/dev/null";
    int result = system(command.c_str());
    return result == 0;
}

// Find main script in extracted directory
std::string FindMainScript(const std::string& dir) {
    std::vector<std::string> candidates = {"main.lua", "game.lua", "init.lua", "run.lua"};
    
    for (const auto& candidate : candidates) {
        std::string path = dir + "/" + candidate;
        if (fs::exists(path)) {
            return candidate;
        }
    }
    
    // Search recursively
    for (const auto& entry : fs::recursive_directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            std::string path = entry.path().string();
            if (path.find(".lua") != std::string::npos) {
                return path;
            }
        }
    }
    
    return "";
}

// Load game config from extracted directory
GameConfig LoadGameConfig(const std::string& dir) {
    GameConfig config;
    
    // Try config.json
    std::string configPath = dir + "/config.json";
    if (fs::exists(configPath)) {
        std::ifstream file(configPath);
        if (file.is_open()) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            config = ParseGameConfig(buffer.str());
        }
    }
    
    // If no config.json, try to find main script
    std::string script = FindMainScript(dir);
    if (!script.empty()) {
        config.mainScript = script;
    }
    
    return config;
}

// Draw launcher screen with drag-drop message
void DrawLauncher(SDL_Renderer* renderer, const char* message, bool hasFile = false) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 30, 255);
    SDL_RenderClear(renderer);
    
    // Draw border
    SDL_Rect border = {50, 50, 700, 500};
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderRect(renderer, &border);
    
    // Draw inner area
    SDL_Rect inner = {55, 55, 690, 490};
    if (hasFile) {
        SDL_SetRenderDrawColor(renderer, 50, 100, 50, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 30, 30, 50, 255);
    }
    SDL_RenderFillRect(renderer, &inner);
    
    // Draw message text (simplified - would need SDL_ttf for real text)
    if (hasFile) {
        // Draw green checkmark approximation
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_Rect check = {350, 200, 100, 100};
        SDL_RenderFillRect(renderer, &check);
    }
}

// Main launcher loop - returns true if game should start
bool RunLauncher(Engine& engine) {
    SDL_Event event;
    std::string droppedFile;
    bool fileDropped = false;
    
    while (true) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                return false;
            }
            
            if (event.type == SDL_EVENT_DROP_FILE) {
                const char* file = event.drop.data;
                if (file) {
                    std::string filename(file);
                    if (filename.find(".zip") != std::string::npos) {
                        droppedFile = filename;
                        fileDropped = true;
                    }
                }
            }
            
            if (event.type == SDL_EVENT_KEY_DOWN) {
                if (event.key.key == SDLK_ESCAPE) {
                    return false;
                }
                if (event.key.key == SDLK_RETURN && fileDropped) {
                    return true;
                }
            }
        }
        
        DrawLauncher(engine.GetRenderer(), 
            fileDropped ? "Archivo detectado! Presiona ENTER para continuar" 
                        : "Arrastra un archivo .zip aqui para ejecutar el juego",
            fileDropped);
        
        SDL_RenderPresent(engine.GetRenderer());
        SDL_Delay(16);
    }
}

} // namespace Lumina

int main(int argc, char* argv[]) {
    std::cout << "========================================" << std::endl;
    std::cout << "    Lumina Engine v1.0.0" << std::endl;
    std::cout << "    SDL3 + Box2D + Lua/sol2" << std::endl;
    std::cout << "========================================" << std::endl;
    std::cout << std::endl;
    
    // Default launcher size
    const int LAUNCHER_WIDTH = 800;
    const int LAUNCHER_HEIGHT = 600;
    
    Lumina::Engine engine;
    
    // Initialize with launcher resolution
    if (!engine.Initialize("Lumina Engine - Launcher", LAUNCHER_WIDTH, LAUNCHER_HEIGHT)) {
        std::cerr << "Error: Could not initialize the engine" << std::endl;
        return -1;
    }
    
    // Check if a game zip was passed as argument
    std::string gameZipPath;
    if (argc > 1) {
        gameZipPath = argv[1];
    } else {
        // Run launcher and wait for user to drop a file
        bool shouldRunGame = Lumina::RunLauncher(engine);
        engine.Shutdown();
        
        if (!shouldRunGame) {
            return 0;
        }
        
        // In a real implementation, we would get the dropped file path
        // For now, use command line argument or default
        std::cout << "Usage: LuminaEngineExample <game.zip>" << std::endl;
        return 0;
    }
    
    // Extract game zip to temp directory
    std::string tempDir = "/tmp/lumina_game_" + std::to_string(time(nullptr()));
    fs::create_directories(tempDir);
    
    std::cout << "Extracting game: " << gameZipPath << std::endl;
    
    if (!Lumina::ExtractZip(gameZipPath, tempDir)) {
        std::cerr << "Error: Failed to extract game zip" << std::endl;
        fs::remove_all(tempDir);
        return -1;
    }
    
    // Load game configuration
    Lumina::GameConfig config = Lumina::LoadGameConfig(tempDir);
    
    std::cout << "Game: " << config.gameName << std::endl;
    std::cout << "Resolution: " << config.width << "x" << config.height << std::endl;
    std::cout << "Main script: " << config.mainScript << std::endl;
    
    // Shutdown launcher and restart with game resolution
    engine.Shutdown();
    
    // Re-initialize with game resolution
    if (!engine.Initialize(config.gameName, config.width, config.height)) {
        std::cerr << "Error: Could not initialize the engine" << std::endl;
        fs::remove_all(tempDir);
        return -1;
    }
    
    // Change to game directory for script loading
    fs::current_path(tempDir);
    
#ifdef LUA_AVAILABLE
    // Load the game script
    if (!engine.LoadScript(config.mainScript)) {
        std::cerr << "Warning: Failed to load game script: " << config.mainScript << std::endl;
    }
#endif
    
    // Run the game
    engine.Run();
    
    // Cleanup
    fs::remove_all(tempDir);
    
    return 0;
}
