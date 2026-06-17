# Realmbound Wilds Technical Plan

## Project Direction

Realmbound Wilds will be built on a small project-specific custom engine, not Unity, Unreal Engine, Godot, or another commercial/game engine. The engine exists to serve this survival crafting game and its vertical slice. It should stay small, runnable, and easy for a solo developer to understand.

The engine should support the systems Realmbound Wilds needs:

- 3D rendering.
- Window and input handling.
- Scene/world management.
- Entity/component-style gameplay architecture.
- Basic physics and collision.
- Terrain or world chunks later.
- Resource nodes.
- Player controller.
- Interaction raycasts.
- Inventory, crafting, and building systems.
- Enemy AI.
- Saving/loading.
- Debug tools.
- Data-driven definitions for items, recipes, resources, enemies, buildables, biomes, and progression.

## Recommended Stack

- Language: C++20.
- Build system: CMake.
- Window/input: SDL3 or GLFW, with the first bootstrap choosing one explicitly before implementation.
- Rendering: OpenGL first, unless a concrete repository need makes Vulkan worth the extra complexity.
- Debug/editor UI: Dear ImGui once a window and renderer exist.
- Data files: JSON or TOML for early item, recipe, resource, enemy, buildable, biome, and progression definitions.
- Optional later libraries: glm for math, stb_image for image loading, miniaudio or OpenAL for audio, Jolt or Bullet for physics, and an EnTT-style ECS if a lightweight in-house component model becomes limiting.

Dependencies should be added only when they directly reduce risk or keep the custom engine focused on game-specific work. The goal is not to hand-write the operating system layer, image codecs, physics solver, or debug UI from scratch.

## Architecture Rules

- Keep `Engine` and `Game` code separate.
- Keep the engine reusable enough for Realmbound Wilds systems, but not over-generalized.
- Every milestone should leave the project compiling or clearly document what remains incomplete.
- Keep the first version extremely small and runnable.
- Add documentation as systems are introduced.
- Add debug tools early.
- Use placeholder assets and primitive debug rendering first.
- Avoid broad systems before their milestone.
- Prioritize a working vertical slice pipeline over theoretical architecture.

## Repository Structure

```text
Engine/
  Core/
  Platform/
  Renderer/
  Input/
  Scene/
  ECS/
  Physics/
  Assets/
  Audio/
  Debug/
  Math/
Game/
  Source/
    Player/
    Interaction/
    Inventory/
    Items/
    Crafting/
    Building/
    Resources/
    Combat/
    AI/
    Biomes/
    Progression/
    SaveLoad/
  Data/
    Items/
    Recipes/
    Resources/
    Enemies/
    Buildables/
    Biomes/
    Progression/
  Assets/
    Placeholder/
  Scenes/
Editor/
  DebugTools/
Docs/
  Systems/
  Milestones/
External/
Tests/
CMakeLists.txt
AGENTS.md
README.md
```

## Engine Modules

### Core

Owns application startup, shutdown, logging, assertions, time, delta-time, and the main loop. This module should remain dependency-light and provide the foundation other engine modules use.

Planned early types:

- `Application`
- `Logger`
- `Clock`
- `TimeStep`

### Platform

Owns operating-system and library integration for window creation, event polling, and platform services. The first implementation should wrap SDL3 or GLFW behind a small interface so game code does not depend directly on the platform library.

Planned early types:

- `Window`
- `PlatformEvents`
- `PlatformConfig`

### Input

Owns keyboard, mouse, and later controller state. Input should be polled centrally and exposed through a simple query API. Later milestones can add action mapping when the control scheme stabilizes.

Planned early types:

- `InputState`
- `InputSystem`
- `InputAction` later.

### Renderer

Starts with a blank screen, clear color, primitive shapes, debug grid, and basic camera support. OpenGL should be the first rendering backend unless there is a strong reason to choose Vulkan. Rendering should support placeholder primitives before asset loading.

Planned early types:

- `Renderer`
- `RenderDevice`
- `Camera`
- `DebugDraw`
- `Mesh` later.

### Scene

Owns scene objects, transforms, update order, and test scene loading. It should support a small number of objects first and avoid complicated streaming until world chunks become a real need.

Planned early types:

- `Scene`
- `SceneObject`
- `Transform`

### ECS

Use an entity/component-style architecture, but keep the first version simple. A small in-house entity ID plus components may be enough for early milestones. Consider EnTT only if the custom component model starts slowing development.

Planned early types:

- `Entity`
- `ComponentStorage` later.
- `System` later.

### Physics

Start with basic collision/raycast support for interaction and grounded movement. Full rigid-body simulation is not required for the first milestones. Consider Jolt or Bullet later if building placement, combat, and world collision need more than simple primitives.

Planned early types:

- `Ray`
- `RaycastHit`
- `Collider`
- `PhysicsWorld`

### Assets

Owns loading game data and, later, textures, meshes, shaders, and audio. Early milestones should use data files and primitive shapes before introducing a full asset pipeline.

Planned early types:

- `DataRegistry`
- `AssetPath`
- `JsonDataSource` or `TomlDataSource` later.

### Debug

Owns developer-facing tools: logging output, performance counters, debug overlay, debug draw, and test controls. Dear ImGui should be added once the window/render loop exists.

Planned early types:

- `DebugOverlay`
- `DebugConsole` later.
- `DebugStats`

### Math

Owns vector, matrix, quaternion, and transform math wrappers. Prefer a small library such as glm instead of writing fragile math code by hand, unless the first bootstrap only needs tiny local structs.

## Game Modules

### Player

Will own player movement, camera control, health, stamina, sprinting, and player-specific debug readouts. This moves to Milestone 2 in the custom-engine roadmap.

### Interaction

Will own interaction raycasts, interactable interfaces, and debug names for hit objects.

### Inventory And Items

Will load item definitions from data files, manage item stacks, slots, and hotbar state, and provide debug item granting.

### Crafting

Will load recipes from data files, check ingredients, require stations, and produce items through the inventory service.

### Building

Will manage placeable build pieces, preview ghosts, rotation, cost checks, and placement validation.

### Resources

Will manage gatherable resource nodes, drop tables, tool requirements, and gathered item rewards.

### Combat And AI

Will manage damage, enemy state machines, attacks, and loot drops.

### Biomes And Progression

Will manage biome definitions, realm fracture placeholders, shrine objectives, progression flags, and unlocks.

### SaveLoad

Will persist player state, inventory, placed structures, gathered resources, and progression flags once those systems exist.

## Data-Driven Definitions

Use stable string IDs for data so saves and systems can refer to definitions without duplicating content.

Early definition groups:

- Items.
- Recipes.
- Resources.
- Enemies.
- Buildables.
- Biomes.
- Progression flags.

Example fields to support later:

- `id`
- `displayName`
- `description`
- `tags`
- `resourceTier`
- `stackSize`
- `requirements`
- `outputs`
- `unlockFlags`

## Bootstrap Plan

Milestone 0 should create the smallest running engine shell:

1. CMake project with `Engine` and `Game` targets.
2. Window creation.
3. Main loop.
4. Logging.
5. Time and delta-time.
6. Input polling.
7. Blank screen.
8. Debug overlay placeholder.
9. Documentation for how to build and run.

No gameplay systems should be added during Milestone 0.

## Testing And Debug Strategy

- Start with build verification: configure and compile the CMake project.
- Add unit tests for pure engine logic as soon as Core, Math, or data loading exists.
- Add small executable test scenes rather than large hidden fixtures.
- Keep debug controls visible and simple.
- Prefer deterministic test data for inventory, crafting, save/load, and progression.
- Use debug rendering for rays, colliders, transforms, and placement checks before final art.

## Future Multiplayer Boundary

Do not build multiplayer in the vertical slice. Keep future co-op in mind by separating input, simulation state, rendering, and data definitions. Avoid systems that only work because state is hidden inside UI or rendering objects.

