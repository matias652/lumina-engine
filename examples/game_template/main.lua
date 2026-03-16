-- Example Lumina Game Script
-- This is a template for creating games with Lumina Engine

-- Set up the game
print("Game starting: " .. Lumina.Version.get())

-- Example: Get window size
local w, h = Lumina.Window.getSize()
print("Window size: " .. w .. "x" .. h)

-- Create a sprite (example)
-- local player = Lumina.Graphics.createSprite("player.bmp")
-- player:SetPosition(100, 100)
-- player:SetScale(2.0, 2.0)

-- Example: Input handling
function OnUpdate(dt)
    -- Called every frame
    -- dt is delta time in seconds
    
    -- Example: Check for escape to quit
    if Lumina.Input.getKey(Lumina.Key.Escape) then
        Lumina.Application.quit()
    end
    
    -- Example: Move with arrow keys
    -- local speed = 200 * dt
    -- if Lumina.Input.getKey(Lumina.Key.Left) then
    --     player:SetPosition(player:getX() - speed, player:getY())
    -- end
end

function OnRender()
    -- Called every frame after Update
    
    -- Clear screen with dark blue color
    Lumina.Graphics.clear(20, 20, 40)
    
    -- Draw some shapes
    local red = Lumina.Graphics.color(255, 0, 0)
    local green = Lumina.Graphics.color(0, 255, 0)
    local blue = Lumina.Graphics.color(0, 0, 255)
    
    -- Draw a rectangle
    Lumina.Graphics.drawRectFilled(50, 50, 100, 50, red)
    
    -- Draw a circle
    Lumina.Graphics.drawCircleFilled(300, 200, 30, green)
    
    -- Draw a triangle
    Lumina.Graphics.drawTriangleFilled(500, 100, 550, 200, 450, 200, blue)
    
    -- Draw text info
    local fps = Lumina.Time.getFPS()
    print("FPS: " .. fps)
end

-- The game loop will automatically call OnUpdate and OnRender
print("Game initialized successfully!")
