# Player Controller + Interaction

Milestone 2 adds a minimal debug player prototype on top of the Basic 3D Sandbox. It does not add inventory, crafting, building, enemies, combat, terrain generation, procedural worlds, food/survival systems, save/load, or biomes.

## What Exists

- `PlayerController` lives in Game code and owns the temporary player camera.
- `PlayerVitals` tracks health, stamina, sprinting, stamina drain, and delayed regeneration.
- `Interactable` provides a simple named target with position, radius, and debug interaction message.
- `SandboxGame` creates the current test scene with:
  - a debug grid
  - the existing debug cube
  - one interactable `Ancient Stone`
- The debug title shows health, stamina, and the current interactable target or `None`.

## Controls

- `W`: move forward.
- `S`: move backward.
- `A`: strafe left.
- `D`: strafe right.
- `Left Arrow`: look left.
- `Right Arrow`: look right.
- `Up Arrow`: look up.
- `Down Arrow`: look down.
- `Shift`: sprint.
- `E`: interact.
- `Escape`: exit.

Mouse look is still intentionally not implemented. Arrow-key look keeps this milestone small and avoids cursor-capture work before the real player-control pass.

## Health And Stamina

Health is currently fixed at `100`. There is no damage, death, healing, hunger, armor, resistance, or combat yet.

Stamina starts at `100`. Sprinting while moving drains stamina. When sprinting stops, regeneration waits briefly before recovering. Sprinting is blocked once stamina is nearly empty.

These values are prototype tuning only and should not be treated as final gameplay balance.

## Interaction Raycast

Interaction uses a simple math ray from the camera position in the camera forward direction. Each interactable is checked as a sphere using its position and interaction radius.

When the ray points at an interactable within range, the debug title displays its name. Pressing `E` logs the interactable debug message. If no target is found, pressing `E` logs that no target was available.

There is no physics engine, collision scene, object hierarchy, gameplay event system, inventory reward, gathering behavior, or UI prompt yet.

## Build

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Debug
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

1. App opens.
2. Grid and cube still render.
3. Player/camera can move with `WASD`.
4. Arrow keys rotate the view.
5. Holding `Shift` while moving drains stamina.
6. Stamina regenerates after a short delay.
7. Looking at `Ancient Stone` shows its name in the title.
8. Pressing `E` logs/displays an interaction.
9. Escape exits cleanly.

## Intentionally Not Implemented Yet

- Inventory rewards or gathering.
- Crafting, building, combat, enemies, AI, save/load, or biomes.
- Terrain generation or procedural worlds.
- Physics-engine raycasts.
- Final player movement, mouse look, UI, or gameplay balance.

