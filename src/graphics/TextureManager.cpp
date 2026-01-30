#include "LuminaEngine/graphics/TextureManager.h"
#include "LuminaEngine/utils/Logger.h"

namespace Lumina {

static TextureManager* s_instance = nullptr;

void TextureManager::Init(SDL_Renderer* renderer) {
    if (!s_instance) {
        s_instance = new TextureManager();
    }
    s_instance->m_renderer = renderer;
}

void TextureManager::Shutdown() {
    if (s_instance) {
        for (auto& pair : s_instance->m_textures) {
            SDL_DestroyTexture(pair.second);
        }
        s_instance->m_textures.clear();
        delete s_instance;
        s_instance = nullptr;
    }
}

TextureManager& TextureManager::Instance() {
    return *s_instance;
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