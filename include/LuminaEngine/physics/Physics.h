#pragma once

// ============================================================================
// Physics Module - Wrapper for Box2D
// ============================================================================
// This module provides a simplified interface to Box2D physics

#ifdef BOX2D_AVAILABLE
#include <box2d/box2d.h>
#endif

namespace Lumina {

class PhysicsWorld {
public:
    PhysicsWorld();
    ~PhysicsWorld();
    
    void Init(float gravityX = 0.0f, float gravityY = -9.8f);
    void Shutdown();
    
    void Step(float deltaTime);
    
    void SetGravity(float x, float y);
    
    // Body creation (simplified interface)
    int CreateBody(float x, float y, bool isDynamic = true);
    void DestroyBody(int bodyId);
    
    void ApplyForce(int bodyId, float forceX, float forceY);
    void ApplyImpulse(int bodyId, float impulseX, float impulseY);
    
    void GetPosition(int bodyId, float* outX, float* outY);
    void SetPosition(int bodyId, float x, float y);
    
    void GetVelocity(int bodyId, float* outVx, float* outVy);
    void SetVelocity(int bodyId, float vx, float vy);

private:
#ifdef BOX2D_AVAILABLE
    std::unique_ptr<b2World> m_world;
#endif
};

} // namespace Lumina
