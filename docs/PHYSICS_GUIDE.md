# Physics Guide

Lumina Engine integrates Box2D for 2D physics simulation. This guide covers how to use the physics system in your games.

## Table of Contents

- [Overview](#overview)
- [Setup](#setup)
- [Physics World](#physics-world)
- [Bodies](#bodies)
- [Forces and Impulses](#forces-and-impulses)
- [Best Practices](#best-practices)
- [Example](#example)

## Overview

Box2D is a 2D physics engine that provides:
- Rigid body dynamics
- Collision detection
- Joints and constraints
- Continuous collision detection

Lumina Engine provides a simplified wrapper around Box2D for ease of use.

## Setup

### Enable Physics

Physics is enabled by default. To disable:

```bash
cmake -DLUMINA_ENABLE_PHYSICS=OFF ..
```

### Include Header

```cpp
#include <LuminaEngine/physics/Physics.h>
```

## Physics World

The `PhysicsWorld` class manages all physics simulation.

### Initialization

```cpp
Lumina::PhysicsWorld physics;

// Initialize with gravity (in m/s²)
// Default: (0.0, -9.8) - standard Earth gravity
physics.Init(0.0f, -9.8f);
```

### Simulation Step

Call `Step()` in your update loop with the delta time:

```cpp
void Game::Update(float deltaTime) {
    physics.Step(deltaTime);
}
```

> **Important**: Box2D uses meters, not pixels. Use a reasonable scale (e.g., 1 meter = 32 pixels).

### Gravity

```cpp
// Set gravity (x, y) in m/s²
physics.SetGravity(0.0f, -9.8f);

// Disable gravity
physics.SetGravity(0.0f, 0.0f);
```

### Cleanup

```cpp
physics.Shutdown();
```

## Bodies

Bodies are objects that have position, velocity, and respond to forces.

### Creating Bodies

```cpp
// Create a dynamic body (affected by gravity and forces)
int dynamicBody = physics.CreateBody(x, y, true);

// Create a static body (not affected by forces, doesn't move)
int staticBody = physics.CreateBody(x, y, false);
```

### Body Properties

| Property | Method | Description |
|----------|--------|-------------|
| Position | `GetPosition()`, `SetPosition()` | Body location |
| Velocity | `GetVelocity()`, `SetVelocity()` | Movement speed |

```cpp
// Get position
float x, y;
physics.GetPosition(bodyId, &x, &y);

// Set position (teleport)
physics.SetPosition(bodyId, 100.0f, 50.0f);

// Get velocity
float vx, vy;
physics.GetVelocity(bodyId, &vx, &vy);

// Set velocity
physics.SetVelocity(bodyId, 10.0f, 0.0f);
```

### Destroying Bodies

```cpp
physics.DestroyBody(bodyId);
```

## Forces and Impulses

### Force vs Impulse

| Type | Description | Use Case |
|------|-------------|----------|
| **Force** | Applied continuously over time | Gravity, thrust, friction |
| **Impulse** | Instantaneous change in velocity | Jumps, collisions, explosions |

### Applying Force

```cpp
// Apply continuous force (in Newtons)
// Force is divided by mass and applied each frame
physics.ApplyForce(bodyId, forceX, forceY);
```

Example - constant thrust:

```cpp
void OnUpdate(float dt) {
    if (Input.GetKey(Key.W)) {
        // Apply upward thrust
        physics.ApplyForce(shipId, 0.0f, 500.0f);
    }
}
```

### Applying Impulse

```cpp
// Apply instantaneous impulse
// Immediately changes velocity
physics.ApplyImpulse(bodyId, impulseX, impulseY);
```

Example - jump:

```cpp
if (Input.GetKeyDown(Key.Space)) {
    // Jump impulse (instant)
    physics.ApplyImpulse(playerId, 0.0f, 100.0f);
}
```

## Best Practices

### Coordinate Conversion

Box2D uses meters, not pixels. Use a consistent scale:

```cpp
const float PIXELS_PER_METER = 32.0f;

// Convert pixels to meters
float meters = pixels / PIXELS_PER_METER;

// Convert meters to pixels
float pixels = meters * PIXELS_PER_METER;
```

### Simulation Parameters

- **Timestep**: Use fixed timestep (e.g., 1/60 second)
- **Velocity Iterations**: 6-8 for accuracy
- **Position Iterations**: 2-3 for stability

```cpp
physics.Step(1.0f / 60.0f);
```

### Body Count

Keep the number of active bodies reasonable:
- Mobile: < 100 bodies
- Desktop: < 500 bodies

### Sleep

Bodies that stop moving can "sleep" to save CPU:
- Enabled by default in Box2D
- Wake up on collision or force application

## Example

Complete example of a simple physics game:

```cpp
#include <LuminaEngine/core/Engine.h>
#include <LuminaEngine/physics/Physics.h>
#include <LuminaEngine/input/Input.h>
#include <iostream>

class Game {
private:
    Lumina::Engine m_engine;
    Lumina::PhysicsWorld m_physics;
    int m_playerBody;
    
    const float PPM = 32.0f; // Pixels per meter
    
public:
    void Run() {
        if (!m_engine.Initialize("Physics Demo", 1280, 720)) {
            std::cerr << "Failed to initialize" << std::endl;
            return;
        }
        
        // Initialize physics
        m_physics.Init(0.0f, -9.8f * PPM);
        
        // Create player body at position (100, 100) pixels
        m_playerBody = m_physics.CreateBody(
            100.0f / PPM, 
            100.0f / PPM, 
            true
        );
        
        // Create ground
        int ground = m_physics.CreateBody(
            640.0f / PPM,
            680.0f / PPM,
            false  // static
        );
        
        // Set ground velocity to zero (static)
        m_physics.SetVelocity(ground, 0.0f, 0.0f);
        
        // Main loop
        m_engine.Run();
        
        m_physics.Shutdown();
        m_engine.Shutdown();
    }
};

int main() {
    Game game;
    game.Run();
    return 0;
}
```

## Advanced Topics

### Collision Detection

For collision callbacks and advanced collision handling, use Box2D directly:

```cpp
// Access underlying b2World (advanced usage)
b2World* world = static_cast<b2World*>(m_physicsWorld);
```

### Joints

For constraints between bodies (springs, wheels, etc.), access Box2D directly:

```cpp
#include <box2d/box2d.h>

// Create a joint
b2RevoluteJointDef jointDef;
jointDef.bodyA = bodyA;
jointDef.bodyB = bodyB;
jointDef.localAnchorA.Set(0, 0);
jointDef.localAnchorB.Set(0, 0);

b2Joint* joint = world->CreateJoint(&jointDef);
```

> **Note**: Direct Box2D access requires including the Box2D header and is considered advanced usage.

## Troubleshooting

### Bodies pass through each other
- Increase position iterations
- Enable continuous collision detection
- Use smaller timesteps

### Bodies behave strangely
- Check your scale (PPM value)
- Ensure consistent units (meters vs pixels)
- Verify deltaTime is reasonable

### Performance issues
- Reduce number of active bodies
- Disable physics for distant objects
- Use sleep feature

### Objects don't stop moving
- Add damping: `body->SetLinearDamping(1.0f)`
- Add friction to ground
- Check for forces being applied incorrectly
