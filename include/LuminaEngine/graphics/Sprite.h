#pragma once
#include <SDL3/SDL.h>
#include <string>

namespace Lumina {

class Sprite {
public:
    Sprite(const std::string& filename);
    ~Sprite() = default;

    void SetPosition(float x, float y);
    void SetScale(float x, float y);
    void SetRotation(double angle);
    
    // For animations: defines which part of the texture to draw
    void SetRect(int x, int y, int w, int h);
    
    void Draw();

private:
    SDL_Texture* m_texture;
    float m_x = 0.0f, m_y = 0.0f;
    float m_scaleX = 1.0f, m_scaleY = 1.0f;
    double m_rotation = 0.0;
    SDL_FRect m_srcRect;
    bool m_hasSrcRect = false;
};

}