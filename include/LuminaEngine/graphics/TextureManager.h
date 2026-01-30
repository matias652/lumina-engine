#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>

namespace Lumina {

class TextureManager {
public:
    static void Init(SDL_Renderer* renderer);
    static void Shutdown();
    static TextureManager& Instance();

    SDL_Texture* LoadTexture(const std::string& filename);
    SDL_Renderer* GetRenderer() const { return m_renderer; }

private:
    TextureManager() = default;
    SDL_Renderer* m_renderer = nullptr;
    std::unordered_map<std::string, SDL_Texture*> m_textures;
};

}