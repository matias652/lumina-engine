#pragma once

#include <SDL3/SDL.h>
#include <string>
#include <unordered_map>
#include <cstdint>
#include <tuple>
#include <utility>

namespace Lumina {

class Sprite;

class Graphics {
public:
    static void Init(SDL_Renderer* renderer);
    static void Shutdown();

    // Clear and Present
    static void Clear(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    static void Present();

    // Primitives
    static void DrawPoint(float x, float y, uint32_t color);
    static void DrawLine(float x1, float y1, float x2, float y2, uint32_t color);
    static void DrawRect(float x, float y, float w, float h, uint32_t color);
    static void DrawRectFilled(float x, float y, float w, float h, uint32_t color);
    static void DrawCircle(float x, float y, float radius, uint32_t color);
    static void DrawCircleFilled(float x, float y, float radius, uint32_t color);
    static void DrawTriangle(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color);
    static void DrawTriangleFilled(float x1, float y1, float x2, float y2, float x3, float y3, uint32_t color);

    // Color utilities
    static uint32_t Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);
    static uint32_t ColorRGB(uint8_t r, uint8_t g, uint8_t b);
    static uint8_t GetColorR(uint32_t color);
    static uint8_t GetColorG(uint32_t color);
    static uint8_t GetColorB(uint32_t color);
    static uint8_t GetColorA(uint32_t color);

    // Blend modes: 0=none, 1=alpha, 2=additive, 3=multiply
    static void SetBlendMode(int mode);
    static int GetBlendMode();
    static void SetAlpha(uint8_t alpha);
    static uint8_t GetAlpha();

    // Camera/Viewport
    static void SetCamera(float x, float y);
    static std::pair<float, float> GetCamera();
    static void SetZoom(float scale);
    static float GetZoom();
    static void SetViewport(int x, int y, int w, int h);
    static std::tuple<int, int, int, int> GetViewport();

    // Texture management
    static int LoadTexture(const std::string& filename);
    static void UnloadTexture(int id);
    static void DrawTexture(int id, float x, float y);
    static void DrawTextureEx(int id, float x, float y, double angle, float scaleX, float scaleY, bool flipH, bool flipV);
    static std::pair<int, int> GetTextureSize(int id);
    static int GetTextureWidth(int id);
    static int GetTextureHeight(int id);

    // Sprite creation
    static Sprite* CreateSprite(const std::string& filename);

    // Debug
    static void DrawGrid(float spacing, uint32_t color);
    static void DrawAxis(uint32_t colorX, uint32_t colorY);

private:
    static SDL_Renderer* s_renderer;
    static std::unordered_map<int, SDL_Texture*> s_textures;
    static int s_nextTextureId;
    static float s_cameraX, s_cameraY;
    static float s_zoom;
    static int s_viewportX, s_viewportY, s_viewportW, s_viewportH;
    static int s_blendMode;
    static uint8_t s_alpha;
};

}
