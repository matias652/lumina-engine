#include "LuminaEngine/graphics/Graphics.h"
#include "LuminaEngine/graphics/TextureManager.h"
#include "LuminaEngine/graphics/Sprite.h"
#include "LuminaEngine/utils/Logger.h"
#include <cmath>
#include <algorithm>

namespace Lumina {

// Static member definitions
SDL_Renderer* Graphics::s_renderer = nullptr;
std::unordered_map<int, SDL_Texture*> Graphics::s_textures;
int Graphics::s_nextTextureId = 1;
float Graphics::s_cameraX = 0.0f;
float Graphics::s_cameraY = 0.0f;
float Graphics::s_zoom = 1.0f;
int Graphics::s_viewportX = 0;
int Graphics::s_viewportY = 0;
int Graphics::s_viewportW = 0;
int Graphics::s_viewportH = 0;
int Graphics::s_blendMode = 1;
uint8_t Graphics::s_alpha = 255;

void Graphics::Init(SDL_Renderer* renderer) {
    s_renderer = renderer;
    s_nextTextureId = 1;
    s_cameraX = 0.0f;
    s_cameraY = 0.0f;
    s_zoom = 1.0f;
    s_blendMode = 1;
    s_alpha = 255;
    LUMINA_LOG_INFO("Graphics system initialized");
}

void Graphics::Shutdown() {
    for (auto& pair : s_textures) {
        SDL_DestroyTexture(pair.second);
    }
    s_textures.clear();
    s_renderer = nullptr;
    LUMINA_LOG_INFO("Graphics system shutdown");
}

void Graphics::Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, r, g, b, a);
    SDL_RenderClear(s_renderer);
}

void Graphics::Present() {
    if (!s_renderer) return;
    SDL_RenderPresent(s_renderer);
}

void Graphics::DrawPoint(float x, float y, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    SDL_RenderPoint(s_renderer, x, y);
}

void Graphics::DrawLine(float x1, float y1, float x2, float y2, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    SDL_RenderLine(s_renderer, x1, y1, x2, y2);
}

void Graphics::DrawRect(float x, float y, float w, float h, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    SDL_FRect rect = {x, y, w, h};
    SDL_RenderRect(s_renderer, &rect);
}

void Graphics::DrawRectFilled(float x, float y, float w, float h, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    SDL_FRect rect = {x, y, w, h};
    SDL_RenderFillRect(s_renderer, &rect);
}

void Graphics::DrawCircle(float x, float y, float radius, uint32_t color) {
    if (!s_renderer || radius <= 0) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    
    int32_t cx = static_cast<int32_t>(x);
    int32_t cy = static_cast<int32_t>(y);
    int32_t r = static_cast<int32_t>(radius);
    
    for (int32_t dy = -r; dy <= r; dy++) {
        for (int32_t dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy == r * r) {
                SDL_RenderPoint(s_renderer, cx + dx, cy + dy);
            }
        }
    }
}

void Graphics::DrawCircleFilled(float x, float y, float radius, uint32_t color) {
    if (!s_renderer || radius <= 0) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    
    int32_t cx = static_cast<int32_t>(x);
    int32_t cy = static_cast<int32_t>(y);
    int32_t r = static_cast<int32_t>(radius);
    
    for (int32_t dy = -r; dy <= r; dy++) {
        for (int32_t dx = -r; dx <= r; dx++) {
            if (dx * dx + dy * dy <= r * r) {
                SDL_RenderPoint(s_renderer, cx + dx, cy + dy);
            }
        }
    }
}

void Graphics::DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    SDL_RenderLine(s_renderer, x1, y1, x2, y2);
    SDL_RenderLine(s_renderer, x2, y2, x3, y3);
    SDL_RenderLine(s_renderer, x3, y3, x1, y1);
}

void Graphics::DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color) {
    if (!s_renderer) return;
    SDL_SetRenderDrawColor(s_renderer, GetColorR(color), GetColorG(color), GetColorB(color), GetColorA(color));
    
    float minX = std::min({x1, x2, x3});
    float maxX = std::max({x1, x2, x3});
    float minY = std::min({y1, y2, y3});
    float maxY = std::max({y1, y2, y3});
    
    auto pointInTriangle = [](float px, float py, float x1, float y1, float x2, float y2, float x3, float y3) -> bool {
        float d1 = (px - x2) * (y1 - y2) - (x1 - x2) * (py - y2);
        float d2 = (px - x3) * (y2 - y3) - (x2 - x3) * (py - y3);
        float d3 = (px - x1) * (y3 - y1) - (x3 - x1) * (py - y1);
        return (d1 >= 0 && d2 >= 0 && d3 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0);
    };
    
    for (float py = minY; py <= maxY; py++) {
        for (float px = minX; px <= maxX; px++) {
            if (pointInTriangle(px, py, x1, y1, x2, y2, x3, y3)) {
                SDL_RenderPoint(s_renderer, static_cast<float>(px), static_cast<float>(py));
            }
        }
    }
}

uint32_t Graphics::Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | a;
}

uint32_t Graphics::ColorRGB(uint8_t r, uint8_t g, uint8_t b) {
    return Color(r, g, b, 255);
}

uint8_t Graphics::GetColorR(uint32_t color) {
    return static_cast<uint8_t>((color >> 24) & 0xFF);
}

uint8_t Graphics::GetColorG(uint32_t color) {
    return static_cast<uint8_t>((color >> 16) & 0xFF);
}

uint8_t Graphics::GetColorB(uint32_t color) {
    return static_cast<uint8_t>((color >> 8) & 0xFF);
}

uint8_t Graphics::GetColorA(uint32_t color) {
    return static_cast<uint8_t>(color & 0xFF);
}

void Graphics::SetBlendMode(int mode) {
    s_blendMode = mode;
    if (!s_renderer) return;
    
    SDL_BlendMode blend;
    switch (mode) {
        case 0: blend = SDL_BLENDMODE_NONE; break;
        case 1: blend = SDL_BLENDMODE_BLEND; break;
        case 2: blend = SDL_BLENDMODE_ADD; break;
        case 3: blend = SDL_BLENDMODE_MOD; break;
        default: blend = SDL_BLENDMODE_BLEND; break;
    }
    SDL_SetRenderDrawBlendMode(s_renderer, blend);
}

int Graphics::GetBlendMode() {
    return s_blendMode;
}

void Graphics::SetAlpha(uint8_t alpha) {
    s_alpha = alpha;
}

uint8_t Graphics::GetAlpha() {
    return s_alpha;
}

void Graphics::SetCamera(float x, float y) {
    s_cameraX = x;
    s_cameraY = y;
}

std::pair<float, float> Graphics::GetCamera() {
    return {s_cameraX, s_cameraY};
}

void Graphics::SetZoom(float scale) {
    s_zoom = scale;
}

float Graphics::GetZoom() {
    return s_zoom;
}

void Graphics::SetViewport(int x, int y, int w, int h) {
    s_viewportX = x;
    s_viewportY = y;
    s_viewportW = w;
    s_viewportH = h;
    if (s_renderer && w > 0 && h > 0) {
        SDL_Rect rect = {x, y, w, h};
        SDL_SetRenderViewport(s_renderer, &rect);
    }
}

std::tuple<int, int, int, int> Graphics::GetViewport() {
    return {s_viewportX, s_viewportY, s_viewportW, s_viewportH};
}

int Graphics::LoadTexture(const std::string& filename) {
    if (!s_renderer) return -1;
    
    auto it = std::find_if(s_textures.begin(), s_textures.end(),
        [&filename](const std::pair<int, SDL_Texture*>& pair) {
            const char* path = SDL_GetTextureFilename(pair.second);
            return path && filename == path;
        });
    
    if (it != s_textures.end()) {
        return it->first;
    }
    
    SDL_Surface* surface = SDL_LoadBMP(filename.c_str());
    if (!surface) {
        LUMINA_LOG_ERROR("Failed to load texture: %s", SDL_GetError());
        return -1;
    }
    
    SDL_Texture* texture = SDL_CreateTextureFromSurface(s_renderer, surface);
    SDL_DestroySurface(surface);
    
    if (!texture) {
        LUMINA_LOG_ERROR("Failed to create texture: %s", SDL_GetError());
        return -1;
    }
    
    int id = s_nextTextureId++;
    s_textures[id] = texture;
    LUMINA_LOG_INFO("Loaded texture: %s (id=%d)", filename.c_str(), id);
    return id;
}

void Graphics::UnloadTexture(int id) {
    auto it = s_textures.find(id);
    if (it != s_textures.end()) {
        SDL_DestroyTexture(it->second);
        s_textures.erase(it);
    }
}

void Graphics::DrawTexture(int id, float x, float y) {
    DrawTextureEx(id, x, y, 0.0, 1.0, 1.0, false, false);
}

void Graphics::DrawTextureEx(int id, float x, float y, double angle, float scaleX, float scaleY, bool flipH, bool flipV) {
    auto it = s_textures.find(id);
    if (it == s_textures.end() || !s_renderer) return;
    
    SDL_Texture* texture = it->second;
    int w, h;
    SDL_GetTextureSize(texture, &w, &h);
    
    SDL_FRect dst = {x, y, static_cast<float>(w) * scaleX, static_cast<float>(h) * scaleY};
    SDL_FlipMode flip = SDL_FLIP_NONE;
    if (flipH) flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_HORIZONTAL);
    if (flipV) flip = static_cast<SDL_FlipMode>(flip | SDL_FLIP_VERTICAL);
    
    SDL_RenderTextureRotated(s_renderer, texture, nullptr, &dst, angle, nullptr, flip);
}

std::pair<int, int> Graphics::GetTextureSize(int id) {
    auto it = s_textures.find(id);
    if (it == s_textures.end()) return {0, 0};
    
    int w, h;
    SDL_GetTextureSize(it->second, &w, &h);
    return {w, h};
}

int Graphics::GetTextureWidth(int id) {
    auto [w, h] = GetTextureSize(id);
    (void)h;
    return w;
}

int Graphics::GetTextureHeight(int id) {
    auto [w, h] = GetTextureSize(id);
    (void)w;
    return h;
}

Sprite* Graphics::CreateSprite(const std::string& filename) {
    return new Sprite(filename);
}

void Graphics::DrawGrid(float spacing, uint32_t color) {
    if (!s_renderer || spacing <= 0) return;
    
    SDL_Rect vp;
    SDL_GetRenderViewport(s_renderer, &vp);
    
    for (float x = 0; x < vp.w; x += spacing) {
        DrawLine(x + s_cameraX, s_cameraY, x + s_cameraX, vp.h + s_cameraY, color);
    }
    for (float y = 0; y < vp.h; y += spacing) {
        DrawLine(s_cameraX, y + s_cameraY, vp.w + s_cameraX, y + s_cameraY, color);
    }
}

void Graphics::DrawAxis(uint32_t colorX, uint32_t colorY) {
    if (!s_renderer) return;
    
    SDL_Rect vp;
    SDL_GetRenderViewport(s_renderer, &vp);
    
    float centerX = vp.w / 2.0f + s_cameraX;
    float centerY = vp.h / 2.0f + s_cameraY;
    
    DrawLine(centerX, 0, centerX, static_cast<float>(vp.h), colorX);
    DrawLine(0, centerY, static_cast<float>(vp.w), centerY, colorY);
}

}
