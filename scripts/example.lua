-- ============================================================================
-- Lumina Engine: Comprehensive Lua Example
-- This script demonstrates the core capabilities of the Lumina Engine API.
-- ============================================================================

Lumina.Log.Info("Starting enhanced example script...")

-- 1. Initialize State
local rotation = 0
local speed = 200
local playerPos = { x = 640, y = 360 }
local windowWidth, windowHeight = Lumina.GetWindowSize()

-- 2. Load Assets
-- Note: Replace with actual paths if you have assets
-- local sprite = Lumina.Graphics.Sprite.new("assets/logo.svg")

-- ============================================================================
-- Lifecycle: OnUpdate
-- Called every frame for logic calculations
-- ============================================================================
function OnUpdate(dt)
    -- Handle Movement
    if Lumina.Input.GetKey(Lumina.Key.W) or Lumina.Input.GetKey(Lumina.Key.Up) then
        playerPos.y = playerPos.y - speed * dt
    end
    if Lumina.Input.GetKey(Lumina.Key.S) or Lumina.Input.GetKey(Lumina.Key.Down) then
        playerPos.y = playerPos.y + speed * dt
    end
    if Lumina.Input.GetKey(Lumina.Key.A) or Lumina.Input.GetKey(Lumina.Key.Left) then
        playerPos.x = playerPos.x - speed * dt
    end
    if Lumina.Input.GetKey(Lumina.Key.D) or Lumina.Input.GetKey(Lumina.Key.Right) then
        playerPos.x = playerPos.x + speed * dt
    end

    -- Mouse Interaction
    if Lumina.Input.GetMouseButtonDown(0) then
        local mx, my = Lumina.Input.GetMousePosition()
        Lumina.Log.Debug("Mouse Clicked at: " .. mx .. ", " .. my)
        -- Teleport player to mouse on left click
        playerPos.x = mx
        playerPos.y = my
    end

    -- Update Rotation
    rotation = rotation + 90 * dt
    if rotation >= 360 then rotation = 0 end

    -- Exit on Escape
    if Lumina.Input.GetKeyDown(Lumina.Key.Escape) then
        Lumina.Log.Info("Escape pressed. Quitting engine...")
        Lumina.Quit()
    end
end

-- ============================================================================
-- Lifecycle: OnRender
-- Called every frame for drawing
-- ============================================================================
function OnRender()
    -- This is where you would call draw functions
    -- Example:
    -- sprite:SetPosition(playerPos.x, playerPos.y)
    -- sprite:SetRotation(rotation)
    -- sprite:Draw()
end

-- Final Log
Lumina.Log.Info("Configured window: " .. windowWidth .. "x" .. windowHeight)
Lumina.Log.Info("Controls: WASD/Arrows to move, ESC to exit, Click to teleport.")
