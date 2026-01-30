# Scripting Guide

Lumina Engine integrates Lua using the `sol2` library, allowing you to control aspects of the engine and implement game logic via scripts.

## Loading a Script

You can load a Lua script using the `LoadScript` method of the `Engine` class:

```cpp
Lumina::Engine engine;
engine.Initialize("Scripting Example");
engine.LoadScript("assets/scripts/main.lua");
engine.Run();
```

## Lua API

All engine functions are available under the `Lumina` global table in Lua.

### Lifecycle Functions

The engine looks for specific functions in your Lua script to call at specific times:

- `OnUpdate(dt)`: Called every frame. `dt` is the delta time (time elapsed since last frame) in seconds.
- `OnRender()`: Called every frame after the background is cleared, allowing you to draw custom graphics.

### Logging

Use the `Log` table to output messages to the console:

```lua
Lumina.Log.Info("This is an information message")
Lumina.Log.Warning("This is a warning")
Lumina.Log.Error("Something went wrong!")
Lumina.Log.Debug("Debugging information")
```

### Input Handling

The `Input` table provides functions to check for keyboard and mouse input.

#### Keyboard

```lua
if Lumina.Input.GetKey(Lumina.Key.W) then
    -- 'W' key is being held down
end

if Lumina.Input.GetKeyDown(Lumina.Key.Space) then
    -- 'Space' was pressed this frame
end
```

#### Key Constants

Common key codes are available in `Lumina.Key`:
- `Space`, `Escape`, `Enter`
- `W`, `A`, `S`, `D`
- `Up`, `Down`, `Left`, `Right`

#### Mouse

```lua
if Lumina.Input.GetMouseButton(0) then
    -- Left mouse button is down
end

local x, y = Lumina.Input.GetMousePosition()
Lumina.Log.Info("Mouse position: " .. x .. ", " .. y)
```

### Engine Control

```lua
-- Get current window dimensions
local width, height = Lumina.GetWindowSize()

-- Request the engine to quit
Lumina.Quit()
```

## Example Script

```lua
Lumina.Log.Info("Starting Lua script...")

function OnUpdate(dt)
    if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
        Lumina.Quit()
    end
end
```
