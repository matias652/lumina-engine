cmake_minimum_required(VERSION 3.14)
project(LuminaEngine 
    VERSION 1.0.0 
    DESCRIPTION "Lumina Engine - C++ game engine with SDL3, Box2D, and Lua/sol2"
    HOMEPAGE_URL "https://github.com/lumina-engine/lumina-engine"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build configuration
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Compiler optimizations
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO} -O2 -g")
    set(CMAKE_CXX_FLAGS_MINSIZEREL "${CMAKE_CXX_FLAGS_MINSIZEREL} -Os -DNDEBUG")
endif()

# Compilation options
option(LUMINA_BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LUMINA_BUILD_EXAMPLES "Build example applications" ON)
option(LUMINA_BUILD_TESTS "Build tests" OFF)

# ============================================================================
# FetchContent - Download dependencies automatically if not found on the system
# ============================================================================
include(FetchContent)

# SDL3
option(LUMINA_USE_SYSTEM_SDL3 "Use system installed SDL3" ON)
if(LUMINA_USE_SYSTEM_SDL3)
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
        pkg_check_modules(SDL3 sdl3)
    endif()
endif()

if(NOT SDL3_FOUND)
    message(STATUS "SDL3 not found on system, fetching from GitHub...")
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)
    set(SDL_DISABLE_INSTALL_DOCS ON CACHE BOOL "" FORCE)
    FetchContent_Declare(
        SDL3
        GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
        GIT_TAG        preview-3.1.3
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(SDL3)
    set(SDL3_LIBRARIES SDL3::SDL3)
    set(SDL3_INCLUDE_DIRS "")  # Target handles includes
    set(SDL3_FETCHED TRUE)
endif()

# Lua
option(LUMINA_USE_SYSTEM_LUA "Use system installed Lua" ON)
if(LUMINA_USE_SYSTEM_LUA)
    find_package(Lua QUIET)
endif()

if(LUA_FOUND)
    set(LUA_AVAILABLE TRUE)
else()
    message(STATUS "Lua not found on system, fetching from GitHub...")
    FetchContent_Declare(
        lua_src
        GIT_REPOSITORY https://github.com/lua/lua.git
        GIT_TAG        v5.4.6
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(lua_src)
    
    # Build Lua as a static library
    file(GLOB LUA_SOURCES "${lua_src_SOURCE_DIR}/*.c")
    list(REMOVE_ITEM LUA_SOURCES "${lua_src_SOURCE_DIR}/lua.c" "${lua_src_SOURCE_DIR}/luac.c")
    add_library(lua_static STATIC ${LUA_SOURCES})
    target_include_directories(lua_static PUBLIC ${lua_src_SOURCE_DIR})
    if(UNIX)
        target_compile_definitions(lua_static PUBLIC LUA_USE_POSIX)
        target_link_libraries(lua_static PUBLIC m dl)
    endif()
    
    set(LUA_LIBRARIES lua_static)
    set(LUA_INCLUDE_DIR ${lua_src_SOURCE_DIR})
    set(LUA_AVAILABLE TRUE)
    set(LUA_FETCHED TRUE)
endif()

# Box2D
option(LUMINA_USE_SYSTEM_BOX2D "Use system installed Box2D" ON)
if(LUMINA_USE_SYSTEM_BOX2D)
    find_path(BOX2D_INCLUDE_DIR box2d/box2d.h)
    find_library(BOX2D_LIBRARY NAMES box2d)
endif()

if(BOX2D_INCLUDE_DIR AND BOX2D_LIBRARY)
    set(BOX2D_FOUND TRUE)
else()
    message(STATUS "Box2D not found on system, fetching from GitHub...")
    set(BOX2D_BUILD_UNIT_TESTS OFF CACHE BOOL "" FORCE)
    set(BOX2D_BUILD_TESTBED OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG        v2.4.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(box2d)
    set(BOX2D_LIBRARY box2d)
    set(BOX2D_INCLUDE_DIR "")  # Target handles includes
    set(BOX2D_FOUND TRUE)
    set(BOX2D_FETCHED TRUE)
endif()

# sol2 (Header-only)
option(LUMINA_USE_SYSTEM_SOL2 "Use system installed sol2" OFF)

if(LUMINA_USE_SYSTEM_SOL2)
    find_path(SOL2_INCLUDE_DIR sol/sol.hpp)
    if(SOL2_INCLUDE_DIR)
        message(STATUS "Found sol2: ${SOL2_INCLUDE_DIR}")
    else()
        message(FATAL_ERROR "sol2 not found but LUMINA_USE_SYSTEM_SOL2 is ON")
    endif()
else()
    message(STATUS "Fetching sol2 from GitHub...")
    FetchContent_Declare(
        sol2
        GIT_REPOSITORY https://github.com/ThePhD/sol2.git
        GIT_TAG        v3.3.0
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(sol2)
    
    if(sol2_SOURCE_DIR)
        set(SOL2_INCLUDE_DIR "${sol2_SOURCE_DIR}/include")
    endif()
endif()

# EnTT (Entity Component System)
option(LUMINA_USE_SYSTEM_ENTT "Use system installed EnTT" OFF)

if(LUMINA_USE_SYSTEM_ENTT)
    find_package(EnTT QUIET)
endif()

if(NOT TARGET EnTT::EnTT)
    message(STATUS "Fetching EnTT from GitHub...")
    FetchContent_Declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG        v3.13.1
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(entt)
endif()

# nlohmann_json (JSON for Modern C++)
option(LUMINA_USE_SYSTEM_JSON "Use system installed nlohmann_json" OFF)

if(LUMINA_USE_SYSTEM_JSON)
    find_package(nlohmann_json QUIET)
endif()

if(NOT TARGET nlohmann_json::nlohmann_json)
    message(STATUS "Fetching nlohmann_json from GitHub...")
    FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG        v3.11.3
        GIT_SHALLOW    TRUE
    )
    FetchContent_MakeAvailable(json)
endif()

# OpenGL ES
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(GLES2 glesv2)
endif()

if(NOT GLES2_FOUND)
    message(STATUS "GLESv2 not found via PkgConfig")
endif()

# Directories
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/src")

# Framework source files
set(FRAMEWORK_SOURCES
    "${SOURCE_DIR}/core/Engine.cpp"
    "${SOURCE_DIR}/utils/Logger.cpp"
    "${SOURCE_DIR}/input/Input.cpp"
    "${SOURCE_DIR}/graphics/TextureManager.cpp"
    "${SOURCE_DIR}/graphics/Sprite.cpp"
    "${SOURCE_DIR}/audio/Audio.cpp"
    "${SOURCE_DIR}/physics/Physics.cpp"
)

if(LUA_AVAILABLE)
    list(APPEND FRAMEWORK_SOURCES 
        "${SOURCE_DIR}/scripting/ScriptEngine.cpp"
    )
endif()

# Framework library
add_library(LuminaEngine ${FRAMEWORK_SOURCES})

if(LUMINA_BUILD_SHARED_LIBS)
    set_target_properties(LuminaEngine PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
endif()

target_include_directories(LuminaEngine
    PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${SDL3_INCLUDE_DIRS}
)

if(GLES2_FOUND)
    target_include_directories(LuminaEngine PRIVATE ${GLES2_INCLUDE_DIRS})
    target_link_libraries(LuminaEngine PUBLIC ${GLES2_LIBRARIES})
    target_compile_definitions(LuminaEngine PUBLIC LUMINA_GLES_AVAILABLE)
endif()

# Global definitions
target_compile_definitions(LuminaEngine PUBLIC LUMINA_LOGGING_ENABLED)

# Link libraries
target_link_libraries(LuminaEngine
    PUBLIC
        ${SDL3_LIBRARIES}
    INTERFACE
        # EnTT and nlohmann_json are available for consumers to use.
        # They are not yet used internally by LuminaEngine, so INTERFACE
        # avoids inflating the library's own link requirements.
        $<BUILD_INTERFACE:EnTT::EnTT>
        $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
    PRIVATE
        ${SDL3_CFLAGS_OTHER}
)

# Sol2
if(SOL2_INCLUDE_DIR)
    target_include_directories(LuminaEngine PRIVATE ${SOL2_INCLUDE_DIR})
endif()

# Lua
if(LUA_AVAILABLE)
    target_include_directories(LuminaEngine PUBLIC ${LUA_INCLUDE_DIR})
    target_link_libraries(LuminaEngine PUBLIC ${LUA_LIBRARIES})
    target_compile_definitions(LuminaEngine PUBLIC LUA_AVAILABLE)
endif()

# Box2D
if(BOX2D_FOUND)
    if(BOX2D_INCLUDE_DIR)
        target_include_directories(LuminaEngine PUBLIC ${BOX2D_INCLUDE_DIR})
    endif()
    target_link_libraries(LuminaEngine PUBLIC ${BOX2D_LIBRARY})
    target_compile_definitions(LuminaEngine PUBLIC BOX2D_AVAILABLE)
endif()

# Compiler options
if(MSVC)
    target_compile_options(LuminaEngine PRIVATE /W4 /O2)
else()
    target_compile_options(LuminaEngine PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Target properties
set_target_properties(LuminaEngine PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    PUBLIC_HEADER "include/LuminaEngine/LuminaEngine.h"
)

# Installation
include(GNUInstallDirs)
install(TARGETS LuminaEngine
    EXPORT LuminaEngineTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LuminaEngine
)

install(DIRECTORY ${INCLUDE_DIR}/LuminaEngine
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.h"
)

install(EXPORT LuminaEngineTargets
    FILE LuminaEngineTargets.cmake
    NAMESPACE LuminaEngine::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/LuminaEngine
)

# Integration file installation (Linux/Flatpak)
if(UNIX AND NOT APPLE)
    install(FILES "flatpak/org.luminaengine.LuminaEngine.desktop"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/applications
        OPTIONAL
    )
    install(FILES "flatpak/org.luminaengine.LuminaEngine.metainfo.xml"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/metainfo
        OPTIONAL
    )
    # Install SVG icon for the desktop
    install(FILES "assets/logo.svg"
        DESTINATION ${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps
        RENAME "org.luminaengine.LuminaEngine.svg"
    )
    # Install example scripts
    install(DIRECTORY scripts
        DESTINATION ${CMAKE_INSTALL_DATADIR}/lumina-engine
    )
endif()

# Example executable (optional)
if(LUMINA_BUILD_EXAMPLES)
    set(EXAMPLE_SOURCES "${CMAKE_SOURCE_DIR}/examples/sandbox/main.cpp")
    if(EXISTS ${EXAMPLE_SOURCES})
        add_executable(LuminaEngineExample ${EXAMPLE_SOURCES})
        target_link_libraries(LuminaEngineExample PRIVATE LuminaEngine)
        set_target_properties(LuminaEngineExample PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
        )
        install(TARGETS LuminaEngineExample RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif()
endif()

# Configuration messages
message(STATUS "Lumina Engine ${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "SDL3: ${SDL3_VERSION}")
if(LUA_AVAILABLE)
    message(STATUS "Lua: ${LUA_VERSION_STRING}")
endif()
if(BOX2D_FOUND)
    message(STATUS "Box2D: Found")
endif()
if(TARGET EnTT::EnTT)
    message(STATUS "EnTT: Integrated")
endif()
if(TARGET nlohmann_json::nlohmann_json)
    message(STATUS "JSON: Integrated")
endif()
