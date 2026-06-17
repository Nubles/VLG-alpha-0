cmake_minimum_required(VERSION 3.22)

project(RealmboundWilds
    VERSION 0.1.0
    DESCRIPTION "A custom C++ survival crafting game engine and game prototype"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_library(EngineRuntime STATIC
    Engine/Core/Application.cpp
    Engine/Core/Clock.cpp
    Engine/Core/GameLayer.cpp
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
    target_link_libraries(EngineRuntime PUBLIC user32 gdi32 opengl32)
else()
    message(FATAL_ERROR "Milestone 0 currently provides a native Win32 platform backend only.")
endif()

add_library(GameRuntime STATIC
    Game/Source/Building/BuildableDatabase.cpp
    Game/Source/Building/BuildPlacementController.cpp
    Game/Source/Crafting/CraftingService.cpp
    Game/Source/Crafting/RecipeDatabase.cpp
    Game/Source/Crafting/RecipeDefinition.cpp
    Game/Source/Interactable.cpp
    Game/Source/Inventory/Hotbar.cpp
    Game/Source/Inventory/Inventory.cpp
    Game/Source/Items/ItemDatabase.cpp
    Game/Source/Items/ItemDefinition.cpp
    Game/Source/PlayerController.cpp
    Game/Source/PlayerVitals.cpp
    Game/Source/Resources/DropTable.cpp
    Game/Source/Resources/GatherableNode.cpp
    Game/Source/SandboxGame.cpp
)

target_link_libraries(GameRuntime PUBLIC EngineRuntime)

add_executable(RealmboundWilds
    Game/Source/Main.cpp
)

target_link_libraries(RealmboundWilds PRIVATE GameRuntime)

add_executable(EngineCoreTests
    Tests/EngineCoreTests.cpp
)

target_link_libraries(EngineCoreTests PRIVATE EngineRuntime GameRuntime)

add_test(NAME EngineCoreTests COMMAND EngineCoreTests)

add_executable(GameCoreTests
    Tests/GameCoreTests.cpp
)

target_link_libraries(GameCoreTests PRIVATE GameRuntime)

add_test(NAME GameCoreTests COMMAND GameCoreTests)
