-- ==================================================================================
-- Lumina Engine - Lua API Reference (Current Implementation)
-- ==================================================================================
-- This file documents the functions available in the Lumina Engine Lua environment.

Lumina = {
    Log = {},
    Input = {},
    Graphics = {},
    Key = {}
}

-- ==================================================================================
-- Logging (Lumina.Log)
-- ==================================================================================

-- Logs an info message to the console.
function Lumina.Log.Info(message) end

-- Logs a warning message to the console.
function Lumina.Log.Warning(message) end

-- Logs an error message to the console.
function Lumina.Log.Error(message) end

-- Logs a debug message to the console.
function Lumina.Log.Debug(message) end


-- ==================================================================================
-- Input (Lumina.Input)
-- ==================================================================================

-- Checks if a key is currently held.
-- @param scancode: Use Lumina.Key constants.
function Lumina.Input.GetKey(scancode) end

-- Checks if a key was pressed this frame.
function Lumina.Input.GetKeyDown(scancode) end

-- Checks if a key was released this frame.
function Lumina.Input.GetKeyUp(scancode) end

-- Mouse button states (0: Left, 1: Middle, 2: Right)
function Lumina.Input.GetMouseButton(button) end
function Lumina.Input.GetMouseButtonDown(button) end
function Lumina.Input.GetMouseButtonUp(button) end

-- Gets mouse position as two values.
-- @return x, y
function Lumina.Input.GetMousePosition() end

-- Helper functions for single coordinates.
function Lumina.Input.GetMouseX() end
function Lumina.Input.GetMouseY() end


-- ==================================================================================
-- Key Constants (Lumina.Key)
-- ==================================================================================
-- Common constants: Space, Escape, W, A, S, D, Up, Down, Left, Right, Enter, 
-- LShift, RShift, LCtrl, Alpha0-Alpha9


-- ==================================================================================
-- Graphics (Lumina.Graphics)
-- ==================================================================================

-- Sprite Object
-- Usage: local player = Lumina.Graphics.Sprite.new("assets/player.png")
Lumina.Graphics.Sprite = {
    -- Creates a new sprite.
    new = function(path) return sprite_object end,
    
    -- Sprite methods:
    -- player:SetPosition(x, y)
    -- player:SetScale(sx, sy)
    -- player:SetRotation(angle)
    -- player:SetRect(x, y, w, h)
    -- player:Draw()
}


-- ==================================================================================
-- Global Functions (Lumina)
-- ==================================================================================

-- Request the application to quit.
function Lumina.Quit() end

-- Gets window dimensions.
-- @return width, height
function Lumina.GetWindowSize() end

-- Gets current time in seconds since start.
function Lumina.GetTime() end