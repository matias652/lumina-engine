#include "LuminaEngine/graphics/Sprite.h"
#include "LuminaEngine/graphics/TextureManager.h"

namespace Lumina {

Sprite::Sprite(const std::string& filename) {
    m_texture = TextureManager::Instance().LoadTexture(filename);
    m_srcRect = {0, 0, 0, 0};
}

void Sprite::SetPosition(float x, float y) {
    m_x = x;
    m_y = y;
}

void Sprite::SetScale(float x, float y) {
    m_scaleX = x;
    m_scaleY = y;
}

void Sprite::SetRotation(double angle) {
    m_rotation = angle;
}

void Sprite::SetRect(int x, int y, int w, int h) {
    m_srcRect.x = static_cast<float>(x);
    m_srcRect.y = static_cast<float>(y);
    m_srcRect.w = static_cast<float>(w);
    m_srcRect.h = static_cast<float>(h);
    m_hasSrcRect = true;
}

void Sprite::Draw() {
    if (!m_texture) return;

    SDL_Renderer* renderer = TextureManager::Instance().GetRenderer();
    
    float w, h;
    if (m_hasSrcRect) {
        w = m_srcRect.w;
        h = m_srcRect.h;
    } else {
        SDL_GetTextureSize(m_texture, &w, &h);
    }

    const SDL_FRect dstRect = { m_x, m_y, w * m_scaleX, h * m_scaleY };
    const SDL_FRect* src = m_hasSrcRect ? &m_srcRect : nullptr;

    SDL_RenderTextureRotated(renderer, m_texture, src, &dstRect, m_rotation, nullptr, SDL_FLIP_NONE);
}

}