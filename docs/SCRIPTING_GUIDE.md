# Scripting Guide

Lumina Engine integrates Lua using the `sol2` library, allowing you to control aspects of the engine and implement game logic via scripts.

## Table of Contents

- [Loading a Script](#loading-a-script)
- [Lua API](#lua-api)
  - [Lifecycle Functions](#lifecycle-functions)
  - [Logging](#logging)
  - [Input Handling](#input-handling)
  - [Time](#time)
  - [Engine Control](#engine-control)
  - [Graphics](#graphics)
- [Key Constants](#key-constants)
- [Example Scripts](#example-scripts)

## Loading a Script

You can load a Lua script using the `LoadScript` method of the `Engine` class:

```cpp
#include <LuminaEngine/core/Engine.h>

Lumina::Engine engine;
engine.Initialize("Scripting Example");
engine.LoadScript("scripts/main.lua");
engine.Run();
```

Scripts are loaded relative to the working directory. For debugging, use absolute paths:

```cpp
engine.LoadScript("/full/path/to/scripts/main.lua");
```

## Lua API

All engine functions are available under the `Lumina` global table in Lua.

### Lifecycle Functions

The engine looks for specific functions in your Lua script to call at specific times:

| Function | Description |
|----------|-------------|
| `OnUpdate(dt)` | Called every frame. `dt` is delta time in seconds. |
| `OnRender()` | Called every frame after update, for custom rendering. |

```lua
function OnUpdate(dt)
    -- dt = time since last frame in seconds
    -- Game logic here
end

function OnRender()
    -- Custom drawing here
end
```

### Logging

Use the `Lumina.Log` table to output messages:

| Function | Description |
|----------|-------------|
| `Lumina.Log.Info(msg)` | Information message |
| `Lumina.Log.Warning(msg)` | Warning message |
| `Lumina.Log.Error(msg)` | Error message |
| `Lumina.Log.Debug(msg)` | Debug message |

```lua
Lumina.Log.Info("Game started")
Lumina.Log.Warning("Low health!")
Lumina.Log.Error("Failed to load texture")
Lumina.Log.Debug("Player position: " .. playerX)
```

### Input Handling

The `Lumina.Input` table provides functions to check keyboard and mouse input.

#### Keyboard Functions

| Function | Description |
|----------|-------------|
| `Lumina.Input.GetKey(scancode)` | Returns true while key is held down |
| `Lumina.Input.GetKeyDown(scancode)` | Returns true on frame key was pressed |
| `Lumina.Input.GetKeyUp(scancode)` | Returns true on frame key was released |

#### Mouse Functions

| Function | Description |
|----------|-------------|
| `Lumina.Input.GetMouseButton(button)` | Returns true while mouse button held |
| `Lumina.Input.GetMouseButtonDown(button)` | Returns true on frame button pressed |
| `Lumina.Input.GetMouseButtonUp(button)` | Returns true on frame button released |
| `Lumina.Input.GetMousePosition()` | Returns `x, y` mouse coordinates |
| `Lumina.Input.GetMouseX()` | Returns mouse X coordinate |
| `Lumina.Input.GetMouseY()` | Returns mouse Y coordinate |

```lua
-- Keyboard examples
if Lumina.Input.GetKey(Lumina.Key.W) then
    -- W key is being held
end

if Lumina.Input.GetKeyDown(Lumina.Key.Space) then
    -- Space was pressed this frame
end

if Lumina.Input.GetKeyUp(Lumina.Key.Enter) then
    -- Enter was released this frame
end

-- Mouse examples
if Lumina.Input.GetMouseButton(0) then
    -- Left mouse button is down
end

local x, y = Lumina.Input.GetMousePosition()
Lumina.Log.Info("Mouse: " .. x .. ", " .. y)
```

### Time

| Function | Description |
|----------|-------------|
| `Lumina.GetTime()` | Returns elapsed time in seconds since engine start |

```lua
local elapsed = Lumina.GetTime()
Lumina.Log.Info("Game time: " .. elapsed .. " seconds")
```

### Engine Control

| Function | Description |
|----------|-------------|
| `Lumina.Quit()` | Request the engine to quit |
| `Lumina.GetWindowSize()` | Returns `width, height` of the window |

```lua
-- Get window dimensions
local width, height = Lumina.GetWindowSize()
Lumina.Log.Info("Window: " .. width .. "x" .. height)

-- Quit the game
if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
    Lumina.Quit()
end
```

### Graphics

The `Lumina.Graphics` table provides sprite rendering capabilities:

```lua
-- Create a sprite
local player = Lumina.Graphics.Sprite("assets/player.png")

-- Set properties
player:SetPosition(100, 200)
player:SetScale(2.0, 2.0)
player:SetRotation(45)
player:SetRect(0, 0, 64, 64)

-- Draw the sprite
player:Draw()
```

| Method | Description |
|--------|-------------|
| `sprite:SetPosition(x, y)` | Set sprite position |
| `sprite:SetScale(scaleX, scaleY)` | Set sprite scale |
| `sprite:SetRotation(degrees)` | Set rotation in degrees |
| `sprite:SetRect(x, y, w, h)` | Set source rectangle |
| `sprite:Draw()` | Render the sprite |

## Key Constants

All SDL scancodes are available via `Lumina.Key`:

### Movement Keys
```lua
Lumina.Key.W, Lumina.Key.A, Lumina.Key.S, Lumina.Key.D
Lumina.Key.Up, Lumina.Key.Down, Lumina.Key.Left, Lumina.Key.Right
```

### Function Keys
```lua
Lumina.Key.Space, Lumina.Key.Escape, Lumina.Key.Enter
```

### Modifiers
```lua
Lumina.Key.LShift, Lumina.Key.RShift
Lumina.Key.LCtrl,  Lumina.Key.RCtrl
```

### Numbers
```lua
Lumina.Key.Alpha0, Lumina.Key.Alpha1, ... Lumina.Key.Alpha9
```

### Mouse Buttons
```lua
0 = Left button
1 = Right button
2 = Middle button
```

## Example Scripts

### Basic Game Loop

```lua
Lumina.Log.Info("Starting game...")

function OnUpdate(dt)
    -- Movement
    local speed = 200
    
    if Lumina.Input.GetKey(Lumina.Key.A) then
        Lumina.Log.Debug("Moving left")
    end
    if Lumina.Input.GetKey(Lumina.Key.D) then
        Lumina.Log.Debug("Moving right")
    end
    
    -- Quit with Escape
    if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
        Lumina.Log.Info("Quitting game...")
        Lumina.Quit()
    end
end
```

### Mouse Follower

```lua
local sprite = nil

function OnUpdate(dt)
    if not sprite and Lumina.Input.GetMouseButton(0) then
        -- Create sprite on first click
        sprite = Lumina.Graphics.Sprite("assets/circle.png")
        sprite:SetScale(0.5, 0.5)
    end
    
    if sprite then
        local x, y = Lumina.Input.GetMousePosition()
        sprite:SetPosition(x, y)
    end
end

function OnRender()
    if sprite then
        sprite:Draw()
    end
end
```

### Timer Example

```lua
local startTime = Lumina.GetTime()
local gameDuration = 60 -- seconds

function OnUpdate(dt)
    local elapsed = Lumina.GetTime() - startTime
    local remaining = gameDuration - elapsed
    
    if remaining <= 0 then
        Lumina.Log.Info("Time's up!")
        Lumina.Quit()
    end
end
```
