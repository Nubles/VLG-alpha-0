# Engine Bootstrap

Milestone 0 creates the smallest runnable custom engine shell for Realmbound Wilds. It does not include gameplay, inventory, crafting, building, AI, terrain, or procedural generation.

## What Exists

- `EngineRuntime` static library.
- `RealmboundWilds` game executable.
- Engine/Game separation through CMake targets.
- Native Win32 window backend.
- Main loop.
- Delta-time clock.
- Console logging.
- Input polling for close and Escape.
- Blank screen clear.
- Clean shutdown path.
- Debug overlay placeholder in the window title.
- `EngineCoreTests` executable for pure input-state behavior.

## Current Platform Decision

The bootstrap uses native Win32 APIs instead of SDL3 or GLFW so the first milestone has no downloaded dependencies. A platform abstraction already wraps the window, so SDL3 or GLFW can replace the backend later without changing game code.

## Build

From the repository root:

```powershell
cmake -S . -B build
cmake --build build
```

For the same Windows command shape used by CI:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

The current environment used by Codex did not have `cmake` available on PATH, so local build verification could not be completed here.

## Run

After building:

```powershell
.\build\Debug\RealmboundWilds.exe
```

Depending on the selected CMake generator and configuration, the executable may be under a different configuration folder such as `build\Release`.

## Test

Run the core test executable:

```powershell
.\build\Debug\EngineCoreTests.exe
```

Manual Milestone 0 checks:

1. The application opens a window titled `Realmbound Wilds - Engine Bootstrap`.
2. The window shows a blank dark screen.
3. The title updates with delta-time, FPS, and frame count.
4. Closing the window exits cleanly.
5. Pressing Escape exits cleanly.
6. Console logs show startup and shutdown messages.
