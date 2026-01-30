#pragma once

// ============================================================================
// Lumina Engine - Main Include Header
// Version 1.0.0
// ============================================================================

// Core
#include "core/Engine.h"

// Graphics
#include "graphics/Sprite.h"
#include "graphics/TextureManager.h"

// Input
#include "input/Input.h"

// Scripting (conditional)
#ifdef LUA_AVAILABLE
#include "scripting/ScriptEngine.h"
#endif

// Utilities
#include "utils/Logger.h"

namespace Lumina {

// Version information
constexpr int VERSION_MAJOR = 1;
constexpr int VERSION_MINOR = 0;
constexpr int VERSION_PATCH = 0;

} // namespace Lumina
