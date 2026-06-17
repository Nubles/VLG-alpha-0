# Basic 3D Sandbox

Milestone 1 turns the blank bootstrap window into a tiny 3D sandbox for primitive debug rendering. It does not add player gameplay, inventory, crafting, building, enemies, terrain generation, survival systems, or procedural worlds.

## Rendering Backend

The sandbox uses native Win32 plus a minimal WGL OpenGL context. The renderer uses fixed-function OpenGL for now so the project can draw a clear color, grid, and cube without external dependencies, shader compilation, model loading, or an asset pipeline.

This is intentionally temporary and small. A later renderer milestone can replace fixed-function drawing with a shader-based OpenGL path while keeping the `Renderer` abstraction.

## What Exists

- `Renderer` creates a WGL OpenGL context.
- The frame clears to a dark color.
- A debug ground grid is drawn procedurally.
- A colored debug cube is drawn procedurally.
- `Vec2`, `Vec3`, `Mat4`, and `Transform` exist in `Engine/Math`.
- `Camera` provides position, yaw/pitch look direction, view matrix, and projection matrix.
- `Scene` stores named objects with transforms and debug primitive types.
- `Game/Source/SandboxGame.*` creates the test scene.

## Camera Controls

- `W`: move forward.
- `S`: move backward.
- `A`: strafe left.
- `D`: strafe right.
- `Left Arrow`: look left.
- `Right Arrow`: look right.
- `Up Arrow`: look up.
- `Down Arrow`: look down.
- `Shift`: move faster.
- `Escape`: exit.

Mouse look is intentionally not implemented yet. Arrow-key look keeps Milestone 1 small and avoids changing cursor capture behavior before player controls exist.

## Build

```powershell
cmake -S . -B build
cmake --build build
```

## Run

```powershell
.\build\Debug\RealmboundWilds.exe
```

## Test

```powershell
.\build\Debug\EngineCoreTests.exe
```

Manual checks:

1. App opens a window titled `Realmbound Wilds - Basic 3D Sandbox`.
2. The screen clears to a dark color.
3. A ground grid is visible.
4. A colored cube is visible above the grid.
5. WASD moves the debug camera.
6. Arrow keys rotate the debug camera.
7. Shift increases movement speed.
8. Escape exits cleanly.

## Intentionally Not Implemented Yet

- Player controller or gameplay.
- Mouse-look capture.
- Inventory, crafting, building, combat, AI, or survival systems.
- Terrain, world chunks, procedural generation, or asset loading.
- Shader-based renderer, materials, textures, mesh assets, or editor tools.

