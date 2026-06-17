cmake_minimum_required(VERSION 3.22)

project(RealmboundWilds
    VERSION 0.1.0
    DESCRIPTION "A custom C++ survival crafting game engine and game prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(EngineRuntime STATIC
    Engine/Core/Application.cpp
    Engine/Core/Clock.cpp
    Engine/Core/Logger.cpp
    Engine/Debug/DebugOverlay.cpp
    Engine/Input/InputState.cpp
    Engine/Math/Mat4.cpp
    Engine/Math/Transform.cpp
    Engine/Math/Vec2.cpp
    Engine/Math/Vec3.cpp
    Engine/Platform/Window.cpp
    Engine/Renderer/Renderer.cpp
    Engine/Scene/Camera.cpp
    Engine/Scene/Scene.cpp
)

target_include_directories(EngineRuntime
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

if(WIN32)
    target_compile_definitions(EngineRuntime PRIVATE RW_PLATFORM_WINDOWS=1)
    target_link_libraries(EngineRuntime PRIVATE user32 gdi32 opengl32)
else()
    message(FATAL_ERROR "Milestone 0 currently provides a native Win32 platform backend only.")
endif()

add_executable(RealmboundWilds
    Game/Source/Main.cpp
    Game/Source/SandboxGame.cpp
)

target_link_libraries(RealmboundWilds PRIVATE EngineRuntime)

add_executable(EngineCoreTests
    Tests/EngineCoreTests.cpp
)

target_link_libraries(EngineCoreTests PRIVATE EngineRuntime)
