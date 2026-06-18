# Vertical Slice Manual Test

This checklist verifies the current Realmbound Wilds custom engine vertical slice without adding new gameplay scope.

## Build, Run, And Test

Configure:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
```

Build:

```powershell
cmake --build build --config Debug
```

Run:

```powershell
.\build\Debug\RealmboundWilds.exe
```

Automated tests:

```powershell
ctest --test-dir build -C Debug --output-on-failure
```

## Controls

- `WASD`: move
- `Arrow keys`: look fallback
- `M`: toggle mouse look
- `Shift`: sprint
- `E`: interact, gather, or complete objective
- `C`: attack
- `B`: toggle build mode
- `1`, `2`, `3`: select buildable
- `Q`, `R`: rotate build preview
- `Enter`: place buildable
- `Backspace`: cancel build mode
- `F1`-`F5`: grant debug resources
- `F6`-`F9`: craft debug recipes
- `F10`: reset Realm Wisp
- `F11`: craft `realm_anchor`
- `F12`: reset progression
- `O`: save
- `P`: load
- `H`: show help
- `Escape`: exit

## Clean Start Setup

1. Start from a clean working copy.
2. Delete any local save file if you need a fresh run.
3. Build the Debug target.
4. Launch `RealmboundWilds.exe`.
5. Confirm the app opens to the debug 3D sandbox with grid, primitive scene objects, and debug text.

## Happy Path

1. Launch the app.
2. Press `H` and confirm the help/debug message appears.
3. Press `M`, move the mouse, and confirm the camera turns. Press `M` again and confirm arrow-key look still works.
4. Gather basic resources from branch, stone, and fiber nodes using `E`.
5. Craft `primitive_tool`.
6. Gather `Cracked Rock`; it should require `primitive_tool`.
7. Craft `camp_bundle` and `workbench_kit`.
8. Enter build mode with `B`.
9. Select/place `camp_marker`.
10. Select/place `workbench_stub`.
11. Fight the `Realm Wisp` with `C` until defeated.
12. Interact with the `Realm Fracture`.
13. If requirements are missing, confirm the message lists `camp_bundle x1 consumed`, `fiber x3 consumed`, `primitive_tool x1 required, kept`, and exact missing amounts.
14. Stabilise the `Realm Fracture`.
15. Craft `realm_anchor`.
16. Press `O` to save.
17. Change state: move, rotate the camera, gather or place something if available.
18. Press `P` to load.
19. Verify restored player position, camera view direction, inventory, placed buildables, depleted gatherables, fracture state, and Realm Wisp state.

## Expected Debug Messages

- Help text includes movement, mouse look, interaction, combat, build, save, load, and exit controls.
- Gathering success names the gathered object or resource result.
- Missing gathering tool feedback names the missing tool.
- Failed fracture interaction lists all objective requirements and exact missing items.
- Fracture completion reports that the Realm Anchor recipe is unlocked.
- Save and load actions report success or failure in the debug message log.

## Expected Failure Checks

- Attempt the Realm Fracture before crafting requirements. It should not complete.
- Attempt the Realm Fracture with `primitive_tool` missing. It should list the tool as required and kept.
- Attempt the Realm Fracture with too little `fiber`. It should list the missing `fiber` amount.
- Attempt Cracked Rock without `primitive_tool`. It should not gather stone.
- Attempt placement without cost items. It should not consume inventory.
- Attempt overlapping placement. It should fail and preserve inventory.

## Known Limitations

- This is a debug prototype with minimal HUD bars and placeholders, not final UI.
- Objective feedback is text/debug output only.
- Save/load uses the current prototype serializer.
- There is no terrain generation, procedural world, audio, VFX, full UI system, or multiplayer.
- Items, recipes, and buildables load from simple text data files; biome layouts, drop tables, objectives, and enemy definitions remain code-defined.
- Build placement, combat, and interaction use simple prototype checks.
- Mouse sensitivity is hardcoded for now; there is no settings UI or input rebinding yet.

## What To Report If It Fails

Include the commit hash, build configuration, exact action taken, expected result, actual debug message, whether the issue happens before or after save/load, and a screenshot if rendering or debug overlay output looks wrong.
