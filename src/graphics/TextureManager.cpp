#include "LuminaEngine/graphics/TextureManager.h"
#include "LuminaEngine/utils/Logger.h"
#include <cassert>

namespace Lumina {

// Meyer's singleton: constructed once, lives for the duration of the program.
// Init() must be called before any texture operations.
static TextureManager& GetInstance() {
    static TextureManager instance;
    return instance;
}

void TextureManager::Init(SDL_Renderer* renderer) {
    GetInstance().m_renderer = renderer;
}

void TextureManager::Shutdown() {
    TextureManager& mgr = GetInstance();
    for (auto& pair : mgr.m_textures) {
        SDL_DestroyTexture(pair.second);
    }
    mgr.m_textures.clear();
    mgr.m_renderer = nullptr;
}

TextureManager& TextureManager::Instance() {
    TextureManager& mgr = GetInstance();
    assert(mgr.m_renderer != nullptr && "TextureManager::Instance() called before Init()");
    return mgr;
}

SDL_Texture* TextureManager::LoadTexture(const std::string& filename) {
    auto it = m_textures.find(filename);
    if (it != m_textures.end()) {
        return it->second;
    }

    // Note: Native SDL3 supports BMP. For PNG/JPG, integrating SDL_image is recommended.
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    if (!surface) {
        LUMINA_LOG_ERROR("Failed to load texture: %s (SDL Error: %s)", filename.c_str(), SDL_GetError());
        return nullptr;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(m_renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        LUMINA_LOG_ERROR("Failed to create texture from surface: %s (SDL Error: %s)", filename.c_str(), SDL_GetError());
        return nullptr;
    }

    m_textures[filename] = texture;
    LUMINA_LOG_INFO("Loaded texture: %s", filename.c_str());
    return texture;
}

}