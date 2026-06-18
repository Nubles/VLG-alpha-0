# Realmbound Wilds

Realmbound Wilds is a custom C++20 survival crafting prototype built on a small project-specific engine. The project does not use Unity, Unreal, Godot, or another general-purpose engine.

The current repository contains a playable debug vertical slice rather than a foundation-only shell. It runs on Windows with a Win32 platform layer, WGL, and fixed-function OpenGL debug rendering.

## Current Prototype Status

The current vertical slice includes:

- custom C++ engine shell
- Engine/Game CMake target separation
- Win32/WGL fixed-function OpenGL renderer
- Mistwood Hollow handcrafted biome
- player movement, stamina, and interaction
- mouse-look toggle with arrow-key fallback
- inventory and hotbar logic
- resource gathering
- crafting
- build placement
- Realm Wisp combat
- Realm Fracture objective
- save/load
- minimal HUD overlay bars and panel placeholders
- dependency-free debug text overlay
- debug help, messages, and title output
- item, recipe, and buildable data loading from `Game/Data`

This is still a debug prototype. It uses primitive rendering and text/debug feedback so the core loop can stay easy to test before final UI, assets, terrain, audio, or larger content work.

## Tech Stack

- C++20
- CMake
- Windows / Win32
- WGL + fixed-function OpenGL
- GitHub Actions Windows CI
- No third-party runtime dependencies

## Build

Configure:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

Build:

```powershell
cmake --build build --config Debug
```

## Run

```powershell
.\build\Debug\RealmboundWilds.exe
```

The build copies `Game/Data` to the executable output folder so runtime data files are available next to the Debug executable.

## Test

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

CI also runs the test executables directly:

- `EngineCoreTests`
- `GameCoreTests`

## Data Files

Current data-loaded definitions:

- `Game/Data/Items/items.txt`
- `Game/Data/Recipes/recipes.txt`
- `Game/Data/Buildables/buildables.txt`

These files are simple dependency-free text formats. C++ fallback definitions remain in place so the prototype can still run if a data file is missing or invalid.

Still code-defined for now:

- Mistwood Hollow biome layout
- gatherable drop tables
- objective definitions
- enemy definitions

## Save File

The quick-save file is written to:

```text
Saves/quick_save.rwsave
```

The path is relative to the process working directory. During normal local runs from the repository root, that means `Saves/quick_save.rwsave` under the repo.

## Useful Docs

- [Developer setup](Docs/Development/DeveloperSetup.md)
- [GitHub Actions and CI guide](Docs/Development/GitHubActions.md)
- [Current project state](Docs/Development/CurrentProjectState.md)
- [Roadmap](ROADMAP.md)
- [Manual QA checklist](Docs/QA/VerticalSliceManualTest.md)
- [Engine bootstrap](Docs/Systems/EngineBootstrap.md)
- [Basic 3D sandbox](Docs/Systems/Basic3DSandbox.md)
- [Player controller and interaction](Docs/Systems/PlayerControllerInteraction.md)
- [Items and inventory](Docs/Systems/ItemsInventory.md)
- [Resource gathering](Docs/Systems/ResourceGathering.md)
- [Crafting](Docs/Systems/Crafting.md)
- [Building placement](Docs/Systems/BuildingPlacement.md)
- [Combat and enemy AI](Docs/Systems/CombatEnemyAI.md)
- [First biome test area](Docs/Systems/FirstBiomeTestArea.md)
- [Shrine objective progression](Docs/Systems/ShrineObjectiveProgression.md)
- [Save/load](Docs/Systems/SaveLoad.md)
- [Vertical slice polish](Docs/Systems/VerticalSlicePolish.md)
- [UI/HUD foundation](Docs/Systems/UIHudFoundation.md)
- [Debug text overlay](Docs/Systems/DebugTextOverlay.md)
- [Mouse look input](Docs/Systems/MouseLookInput.md)
- [Data loading: items](Docs/Systems/DataLoadingItems.md)
- [Data loading: recipes](Docs/Systems/DataLoadingRecipes.md)
- [Data loading: buildables](Docs/Systems/DataLoadingBuildables.md)
- [Data loading consolidation](Docs/Systems/DataLoadingConsolidation.md)

## Next Recommended Phase

Do not jump straight into terrain or procedural generation. The best next decision is either:

- another deliberate architecture pass to keep the current vertical slice maintainable.
- a scoped input/settings planning pass before key rebinding, sensitivity UI, controller support, or mouse-driven UI.

Choose the next milestone only after onboarding docs and CI are current and green.
