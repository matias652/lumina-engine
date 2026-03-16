---@class Lumina
---@field Version Lumina.Version
---@field Application Lumina.Application
---@field Time Lumina.Time
---@field Window Lumina.Window
---@field Input Lumina.Input
---@field Graphics Lumina.Graphics
---@field Audio Lumina.Audio
---@field Physics Lumina.Physics
---@field Key Lumina.Key
---@field Mouse Lumina.Mouse
Lumina = {}

---@class Lumina.Version
Lumina.Version = {}
---@return string
function Lumina.Version.get() end
---@return integer
function Lumina.Version.getMajor() end
---@return integer
function Lumina.Version.getMinor() end
---@return integer
function Lumina.Version.getPatch() end

---@class Lumina.Application
Lumina.Application = {}
function Lumina.Application.quit() end
---@return boolean
function Lumina.Application.isRunning() end
---@param path string
---@return boolean
function Lumina.Application.loadScript(path) end

---@class Lumina.Time
Lumina.Time = {}
---@return number seconds
function Lumina.Time.get() end
---@return number
function Lumina.Time.getDelta() end
---@return number
function Lumina.Time.getFPS() end

---@class Lumina.Window
Lumina.Window = {}
---@return integer, integer
function Lumina.Window.getSize() end
---@param title string
function Lumina.Window.setTitle(title) end
---@return string
function Lumina.Window.getTitle() end
---@param enabled boolean
function Lumina.Window.setFullscreen(enabled) end
---@return boolean
function Lumina.Window.isFullscreen() end
function Lumina.Window.minimize() end
function Lumina.Window.maximize() end

---@class Lumina.Input
Lumina.Input = {}
---@param key integer
---@return boolean
function Lumina.Input.getKey(key) end
---@param key integer
---@return boolean
function Lumina.Input.getKeyDown(key) end
---@param key integer
---@return boolean
function Lumina.Input.getKeyUp(key) end
---@return number, number
function Lumina.Input.getMousePosition() end
---@param button integer
---@return boolean
function Lumina.Input.getMouseButton(button) end
---@param button integer
---@return boolean
function Lumina.Input.getMouseButtonDown(button) end
---@param button integer
---@return boolean
function Lumina.Input.getMouseButtonUp(button) end
---@return number
function Lumina.Input.getMouseX() end
---@return number
function Lumina.Input.getMouseY() end

---@class Lumina.Graphics
Lumina.Graphics = {}

-- Clear and Present
---@param r number
---@param g number
---@param b number
---@param a number?
function Lumina.Graphics.clear(r, g, b, a) end
function Lumina.Graphics.present() end

-- Primitives
---@param x number
---@param y number
---@param color number
function Lumina.Graphics.drawPoint(x, y, color) end
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param color number
function Lumina.Graphics.drawLine(x1, y1, x2, y2, color) end
---@param x number
---@param y number
---@param w number
---@param h number
---@param color number
function Lumina.Graphics.drawRect(x, y, w, h, color) end
---@param x number
---@param y number
---@param w number
---@param h number
---@param color number
function Lumina.Graphics.drawRectFilled(x, y, w, h, color) end
---@param x number
---@param y number
---@param radius number
---@param color number
function Lumina.Graphics.drawCircle(x, y, radius, color) end
---@param x number
---@param y number
---@param radius number
---@param color number
function Lumina.Graphics.drawCircleFilled(x, y, radius, color) end
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param x3 number
---@param y3 number
---@param color number
function Lumina.Graphics.drawTriangle(x1, y1, x2, y2, x3, y3, color) end
---@param x1 number
---@param y1 number
---@param x2 number
---@param y2 number
---@param x3 number
---@param y3 number
---@param color number
function Lumina.Graphics.drawTriangleFilled(x1, y1, x2, y2, x3, y3, color) end

-- Color utilities
---@return number packed RGBA
function Lumina.Graphics.color(r, g, b, a) end
---@return number packed RGBA
function Lumina.Graphics.colorRGB(r, g, b) end
---@param color number
---@return number
function Lumina.Graphics.getColorR(color) end
---@param color number
---@return number
function Lumina.Graphics.getColorG(color) end
---@param color number
---@return number
function Lumina.Graphics.getColorB(color) end
---@param color number
---@return number
function Lumina.Graphics.getColorA(color) end

-- Blend modes
---@param mode number (0=none, 1=alpha, 2=additive, 3=multiply)
function Lumina.Graphics.setBlendMode(mode) end
---@return number
function Lumina.Graphics.getBlendMode() end
---@param alpha number (0-255)
function Lumina.Graphics.setAlpha(alpha) end
---@return number
function Lumina.Graphics.getAlpha() end

-- Camera/Viewport
---@param x number
---@param y number
function Lumina.Graphics.setCamera(x, y) end
---@return number, number
function Lumina.Graphics.getCamera() end
---@param scale number
function Lumina.Graphics.setZoom(scale) end
---@return number
function Lumina.Graphics.getZoom() end
---@param x number
---@param y number
---@param w number
---@param h number
function Lumina.Graphics.setViewport(x, y, w, h) end
---@return number, number, number, number
function Lumina.Graphics.getViewport() end

-- Textures
---@param filename string
---@return number textureId
function Lumina.Graphics.loadTexture(filename) end
---@param id number
function Lumina.Graphics.unloadTexture(id) end
---@param id number
---@param x number
---@param y number
function Lumina.Graphics.drawTexture(id, x, y) end
---@param id number
---@param x number
---@param y number
---@param angle number
---@param scaleX number
---@param scaleY number
---@param flipH boolean
---@param flipV boolean
function Lumina.Graphics.drawTextureEx(id, x, y, angle, scaleX, scaleY, flipH, flipV) end
---@param id number
---@return number, number
function Lumina.Graphics.getTextureSize(id) end
---@param id number
---@return number
function Lumina.Graphics.getTextureWidth(id) end
---@param id number
---@return number
function Lumina.Graphics.getTextureHeight(id) end

-- Sprites
---@param filename string
---@return Lumina.Graphics.Sprite
function Lumina.Graphics.createSprite(filename) end

-- Debug
---@param spacing number
---@param color number
function Lumina.Graphics.drawGrid(spacing, color) end
---@param colorX number
---@param colorY number
function Lumina.Graphics.drawAxis(colorX, colorY) end

---@class Lumina.Graphics.Sprite
local Sprite = {}
---@param x number
---@param y number
function Sprite:setPosition(x, y) end
---@param x number
---@param y number
function Sprite:setScale(x, y) end
---@param angle number degrees
function Sprite:setRotation(angle) end
---@param x number
---@param y number
---@param w number
---@param h number
function Sprite:setRect(x, y, w, h) end
function Sprite:draw() end

---@class Lumina.Audio
Lumina.Audio = {}
---@param filename string
---@return number soundId
function Lumina.Audio.loadSound(filename) end
---@param id number
---@param volume number (0.0-1.0)
function Lumina.Audio.playSound(id, volume) end
---@param filename string
---@return number musicId
function Lumina.Audio.loadMusic(filename) end
---@param id number
---@param loop boolean
function Lumina.Audio.playMusic(id, loop) end
function Lumina.Audio.stopMusic() end
---@param volume number (0.0-1.0)
function Lumina.Audio.setMusicVolume(volume) end
---@param volume number (0.0-1.0)
function Lumina.Audio.setMasterVolume(volume) end

---@class Lumina.Physics
Lumina.Physics = {}
---@param x number
---@param y number
function Lumina.Physics.setGravity(x, y) end
---@return number, number
function Lumina.Physics.getGravity() end
---@param x number
---@param y number
---@param dynamic boolean
---@return number bodyId
function Lumina.Physics.createBody(x, y, dynamic) end
---@param bodyId number
function Lumina.Physics.destroyBody(bodyId) end
---@param bodyId number
---@param fx number
---@param fy number
function Lumina.Physics.applyForce(bodyId, fx, fy) end
---@param bodyId number
---@param ix number
---@param iy number
function Lumina.Physics.applyImpulse(bodyId, ix, iy) end
---@param bodyId number
---@return number, number
function Lumina.Physics.getPosition(bodyId) end
---@param bodyId number
---@param x number
---@param y number
function Lumina.Physics.setPosition(bodyId, x, y) end
---@param bodyId number
---@return number, number
function Lumina.Physics.getVelocity(bodyId) end
---@param bodyId number
---@param vx number
---@param vy number
function Lumina.Physics.setVelocity(bodyId, vx, vy) end

---@class Lumina.Key
Lumina.Key = {
    ---@type number
    Space = 0,
    ---@type number
    Escape = 0,
    ---@type number
    W = 0,
    ---@type number
    A = 0,
    ---@type number
    S = 0,
    ---@type number
    D = 0,
    ---@type number
    Up = 0,
    ---@type number
    Down = 0,
    ---@type number
    Left = 0,
    ---@type number
    Right = 0,
    ---@type number
    Enter = 0,
    ---@type number
    LShift = 0,
    ---@type number
    RShift = 0,
    ---@type number
    LCtrl = 0,
    ---@type number
    Alpha0 = 0,
    ---@type number
    Alpha1 = 0,
    ---@type number
    Alpha2 = 0,
    ---@type number
    Alpha3 = 0,
    ---@type number
    Alpha4 = 0,
    ---@type number
    Alpha5 = 0,
    ---@type number
    Alpha6 = 0,
    ---@type number
    Alpha7 = 0,
    ---@type number
    Alpha8 = 0,
    ---@type number
    Alpha9 = 0,
}

---@class Lumina.Mouse
Lumina.Mouse = {
    ---@type number
    Left = 0,
    ---@type number
    Right = 0,
    ---@type number
    Middle = 0,
    ---@type number
    X1 = 0,
    ---@type number
    X2 = 0,
}
