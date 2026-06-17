# Vertical Slice Stabilisation Pass 2

This pass improves test organisation, manual QA flow, and objective feedback without changing gameplay scope.

## Test Organisation

`GameCoreTests` remains a single executable so CI stays simple, but the source is now split by domain:

- `Tests/Game/TestDebug.cpp`
- `Tests/Game/TestBiomes.cpp`
- `Tests/Game/TestInventory.cpp`
- `Tests/Game/TestGathering.cpp`
- `Tests/Game/TestCrafting.cpp`
- `Tests/Game/TestBuilding.cpp`
- `Tests/Game/TestCombat.cpp`
- `Tests/Game/TestProgressionObjectives.cpp`
- `Tests/Game/TestSaveLoad.cpp`

`Tests/GameCoreTests.cpp` is now only the entry point. `Tests/Game/GameTestSuite.h` declares the domain test functions.

The tests still use plain `assert` and do not introduce a test framework dependency.

## QA Document

Manual vertical-slice verification now lives in `Docs/QA/VerticalSliceManualTest.md`.

It includes build, run, test, clean-start, happy-path, save/load, expected debug message, expected failure, known limitation, and failure-reporting guidance.

## Objective Feedback

Realm Fracture missing-requirement feedback now reports the full requirement list instead of only the first missing item.

The debug message describes:

- `camp_bundle x1 consumed`
- `fiber x3 consumed`
- `primitive_tool x1 required, kept`
- exact missing quantities, such as `Missing: fiber x2, primitive_tool x1`

The completion mechanics are unchanged. Consumed items are still consumed, and `primitive_tool` is still required but kept.

## Known Limitations

- Feedback is still debug text only.
- There is no quest log, HUD rewrite, or objective UI.
- Requirements are still hard-coded in the current prototype objective definition.
- The test suite still uses process-level `assert`, so a failing assertion stops the executable.

## Intentionally Not Changed

- No new gameplay systems.
- No terrain or procedural generation.
- No new biomes, enemies, crafting recipes, buildables, or resources.
- No renderer rewrite.
- No data loading.
- No audio, VFX, multiplayer, or real UI/HUD.

