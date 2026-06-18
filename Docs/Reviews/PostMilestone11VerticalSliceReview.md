# Post-Milestone 11 Vertical Slice Review

## Executive Summary

Milestones 0-11 have produced a coherent custom-engine debug vertical slice. The project now proves the intended loop: spawn in Mistwood Hollow, gather resources, craft, build, fight a Realm Wisp, stabilise the Realm Fracture, unlock/craft the realm_anchor, save, and load.

The strongest next move is to stabilise the current vertical slice before adding the next gameplay feature. The slice works, but `SandboxGame` has become the integration hub for nearly every system, definitions are still mostly hardcoded, and save/load lacks stable IDs for a few world objects. These are normal prototype seams, but they should be addressed before terrain, UI, additional biomes, or expanded combat make the system harder to reshape.

Primary recommendation: choose option A, stabilise the current vertical slice first.

## What Is Working Well

- Engine/Game separation is still mostly clean. Engine code owns generic platform, window, input, renderer, math, scene, and core loop pieces.
- Game systems are split into focused folders: AI, Biomes, Building, Combat, Crafting, Debug, Inventory, Items, Objectives, Progression, Resources, and SaveLoad.
- CMake target separation is simple and useful: `EngineRuntime`, `GameRuntime`, `RealmboundWilds`, `EngineCoreTests`, and `GameCoreTests`.
- GitHub Actions verifies configure, build, and both test executables on Windows.
- Most gameplay rules are covered by pure logic tests in `GameCoreTests`.
- The debug vertical slice has a documented manual path and review checklist.
- The current renderer and scene primitives are sufficient for validating gameplay systems without asset or renderer churn.

## Current Risks

- `Game/Source/SandboxGame.cpp` is now the biggest risk. It is about 630 lines and owns update routing, interaction, build placement, combat, save/load capture and restore, biome loading, debug title formatting, and visual syncing.
- Save/load does not yet persist gatherable depletion. The save docs call this out, but it will become a correctness issue as soon as resource gathering matters across sessions.
- Placed buildable save data stores buildable ID, position, and yaw only. That is acceptable now, but later buildables will need stable instance IDs and possibly health/state.
- Hardcoded definitions are still acceptable for the prototype, but the roadmap previously describes data-loaded items/recipes. The project has reached the point where data-loading should be planned before content expands.
- `GameCoreTests.cpp` is broad and long. It proves coverage, but failures may become harder to localise as more systems are added.
- Input bindings are all hardcoded in the Win32 polling layer. This is fine for now, but future control work should avoid scattering key meaning across platform and game code.

## Architecture Observations

### Engine/Game Separation

The separation is healthy. EngineRuntime still has no item, inventory, objective, biome, combat, or save/load knowledge. Game-side systems depend on engine math/scene/input types where needed, which is reasonable for this prototype.

### SandboxGame Coupling

`SandboxGame` is functioning as the composition root and runtime coordinator. That was appropriate for early milestones, but it now coordinates too many feature workflows:

- interaction target selection
- gather/interact behavior
- debug granting and crafting
- build mode and placed object syncing
- combat and Realm Wisp syncing
- objective/progression interaction
- save/load snapshot capture and restore
- debug title formatting

This should be split carefully, not rewritten wholesale. The first extraction should be small and behavior-preserving.

### Game Systems

Most systems are small and readable. Crafting, building, progression, objectives, save/load, and debug helpers are still understandable as independent units. Biome layout is useful, but it also shows why definitions should eventually move from code to data.

## Build And CI Observations

- CMake is readable and explicit.
- The single Windows workflow is adequate for the current Win32/WGL project.
- Build logs are uploaded on failure, and annotations have been useful.
- Tests run both engine and game executables.
- Later, consider adding stricter warning settings and a local documented Visual Studio generator command, but do not do that before stabilising current code shape.

## Code Organisation Observations

- Folder layout is good for the project stage.
- Debug-only systems are now isolated under `Game/Source/Debug`, which is a positive trend.
- `Game/Data/*` folders exist but are not yet used. That mismatch should be resolved when moving to data loading.
- Some naming is still prototype-oriented but clear: Realm Wisp, Mistwood Hollow, Realm Fracture, camp_bundle, workbench_kit.
- Comments are sparse but acceptable. More useful documentation should go into system docs rather than inline comments unless code becomes non-obvious.

## Vertical Slice Gameplay Flow

The current flow is coherent:

1. Spawn in Mistwood Hollow.
2. Gather wood, stone, fiber, and raw_food.
3. Craft primitive_tool.
4. Gather Cracked Rock.
5. Craft camp_bundle and workbench_kit.
6. Place camp_marker or workbench_stub.
7. Fight Realm Wisp.
8. Stabilise Realm Fracture.
9. Craft realm_anchor.
10. Save and load.

Main rough edges:

- The title can still become long when several recent messages are verbose.
- H help is useful, but it is still console/title debug output rather than a real overlay.
- Objective requirements are not discoverable except by failed interaction.
- Resource depletion is not saved.
- Manual testing is still keyboard-heavy and debug-key-heavy.

## Data And Definitions

Hardcoded definitions remain acceptable for the current reviewable vertical slice. They should not be expanded much further before moving toward a simple data-loading path.

Suggested transition order:

1. Keep current C++ definitions as the authoritative fallback.
2. Add a tiny dependency-free line-based or CSV-like loader for item definitions.
3. Move recipes next.
4. Move buildables after recipes.
5. Move biome layout last, because it has richer object placement needs.

Do not introduce JSON/YAML dependencies yet unless the project intentionally accepts that cost later.

## Save/Load Gaps

Currently persisted:

- save version
- biome ID
- player position
- player health and stamina
- inventory slots
- progression flags
- placed buildables
- Mistwood objective completion
- Realm Fracture stabilised state
- Realm Wisp alive/dead state and position

Not persisted or weakly persisted:

- gatherable depletion
- individual gatherable stable IDs
- placed buildable instance IDs
- hotbar assignments
- current player camera yaw/pitch
- enemy cooldown/state beyond alive and position
- save schema migration beyond version rejection

Before the next big feature, add stable IDs for gatherable nodes and placed buildables, then persist gatherable depletion.

## Test Gaps

Current tests are valuable but concentrated in one large `GameCoreTests.cpp`. Recommended improvements:

- Split game tests by domain once the project has more than one or two failures per change.
- Add tests for applying loaded `SaveData` to runtime state if a narrow harness is introduced.
- Add tests for gatherable stable IDs and depletion persistence when implemented.
- Add tests for malformed inventory/buildable lines beyond the existing invalid save coverage.
- Add a manual smoke-test checklist in the README or a dedicated QA doc for non-developer testers.

## Suggested Refactors

### Must Fix Before Next Big Feature

1. Extract a `VerticalSliceCoordinator` or `SandboxWorldState` from `SandboxGame` to own current runtime state and save/load capture/apply.
2. Add stable IDs to gatherable nodes and placed buildable instances.
3. Persist gatherable depletion.
4. Split `GameCoreTests.cpp` into smaller domain test files or introduce small local test helper functions.
5. Update roadmap language so it reflects the actual current state instead of early milestone assumptions.

### Should Fix Soon

1. Move item and recipe definitions toward simple external data files or a loader-backed fallback.
2. Add camera yaw/pitch to save/load.
3. Clarify objective requirement feedback in debug messages.
4. Create a manual QA checklist document that mirrors the happy path.
5. Add a small developer note for how to inspect GitHub Actions failures.

### Can Defer

1. Renderer cleanup and better debug visuals.
2. Real HUD/UI.
3. Input remapping.
4. Audio/VFX placeholders.
5. Multiple save slots.
6. More enemy types or combat expansion.

### Do Not Touch Yet

1. Procedural generation.
2. Terrain chunks or real terrain mesh.
3. Multiplayer.
4. Large renderer rewrite.
5. Full quest system.
6. Full external data pipeline with third-party dependencies.

## Recommended Next Phase

Choose A: stabilise the current vertical slice first.

Reason: the vertical slice already proves the gameplay direction. Adding a second biome, terrain, real UI, or combat expansion now would increase pressure on the current integration seams. A short stabilisation phase will make every later milestone cheaper and safer by reducing `SandboxGame` coupling, improving save/load identity, and making tests easier to diagnose.

After stabilisation, the next best implementation direction is likely B: real data loading for items, recipes, and buildables. That should happen before major content expansion.

## Prioritised Action List

1. Extract runtime state and save/load capture/apply out of `SandboxGame`.
2. Add stable IDs to gatherables and placed buildables.
3. Persist gatherable depletion.
4. Split `GameCoreTests.cpp` by domain or add domain-local helper sections.
5. Update roadmap and README to reflect the completed milestones and current next phase.
6. Add camera yaw/pitch persistence.
7. Improve objective requirement feedback.
8. Start a minimal data-loading pass for item definitions.
9. Move recipes to data after items.
10. Re-evaluate whether buildables and biome layout should remain code-defined or move to data.

## Stabilisation Pass 1 Result

Completed after this review:

- Added `SandboxWorldState` to own placed buildable identity and gatherable depletion helpers.
- Added deterministic stable IDs to Mistwood Hollow gatherable nodes.
- Added session-local placed buildable instance IDs such as `placed_0001`.
- Save/load now persists depleted gatherable IDs.
- Save/load now persists camera yaw and pitch.
- Placed buildable instance IDs now round-trip through save/load, and the next ID continues after loaded IDs.

Remaining from the original highest-priority list:

- Continue extracting save/load capture/apply and runtime coordination out of `SandboxGame`.
- Consider splitting `GameCoreTests.cpp` by domain.
- Improve objective requirement feedback.
- Start minimal data loading only after the stabilisation work is complete.

## Stabilisation Pass 2 Result

Completed after this review:

- Split `GameCoreTests` into domain-oriented source files under `Tests/Game` while keeping a single CI executable.
- Added `Docs/QA/VerticalSliceManualTest.md` with the current manual happy path, save/load checks, expected failure checks, and reporting guidance.
- Improved Realm Fracture missing-requirement feedback so it lists all requirements, exact missing quantities, and marks `primitive_tool` as required but kept.
- Added focused objective feedback assertions while preserving existing progression, inventory, build, combat, biome, gather, debug, and save/load coverage.
- Added `Docs/Systems/VerticalSliceStabilisationPass2.md`.

Remaining deferred items:

- Continue extracting runtime coordination out of `SandboxGame` only when it stays low risk.
- Start minimal data loading only after stabilisation work is complete.
- Do not begin terrain, procedural generation, renderer rewrite, real UI/HUD, or new gameplay systems yet.

## Stabilisation Pass 3 Result

Completed after this review:

- Added `Game/Data/Items/items.txt` as a pipe-delimited item definition file mirroring the existing starter item definitions.
- Added a tiny dependency-free item parser with validation for field count, empty required fields, known categories, numeric fields, and positive stack sizes.
- Added `ItemDatabase::LoadFromText`, `LoadFromFile`, and `CreateFromFileOrFallback`.
- Updated runtime item database construction to load item definitions from data first and fall back to hardcoded definitions with a warning.
- Added item parser coverage to the inventory/domain tests while preserving existing inventory, crafting, gathering, building, objective, and save/load behavior.
- Added `Docs/Systems/DataLoadingItems.md`.

Remaining deferred items:

- Move recipes to data only after this item-loading pass is reviewed.
- Keep buildables, biome layouts, drops, and objectives code-defined until their own scoped passes.
- Do not begin terrain, procedural generation, renderer rewrite, real UI/HUD, or new gameplay systems yet.

## Stabilisation Pass 4 Result

Completed after this review:

- Added `Game/Data/Recipes/recipes.txt` as a pipe-delimited recipe definition file mirroring the existing starter recipes.
- Added a tiny dependency-free recipe parser with validation for field count, required fields, positive output quantity, ingredient syntax, and positive ingredient quantities.
- Added `RecipeDatabase::LoadFromText`, `LoadFromFile`, and `CreateFromFileOrFallback`.
- Updated runtime recipe database construction to load recipe definitions from data first and fall back to hardcoded definitions with a warning.
- Preserved the `recipe_realm_anchor_unlocked` progression gate for `realm_anchor`.
- Added recipe parser coverage to the crafting/domain tests while preserving existing crafting, gathering, building, objective, inventory, and save/load behavior.
- Added `Docs/Systems/DataLoadingRecipes.md`.

Remaining deferred items:

- Move buildables to data only after this recipe-loading pass is reviewed.
- Keep biome layouts, drops, and objectives code-defined until their own scoped passes.
- Do not begin terrain, procedural generation, renderer rewrite, real UI/HUD, or new gameplay systems yet.

## Stabilisation Pass 6 Result

Completed after this review:

- Added `Game/Source/Data/DataTextParser` to consolidate small text-loading helpers used by item, recipe, and buildable loading.
- Refactored item, recipe, and buildable loaders to use shared helpers for file reading, splitting, comment/blank-line handling, positive number parsing, `Vec3` triples, trimming, and line-error formatting.
- Preserved the existing item, recipe, and buildable data file formats.
- Preserved hardcoded fallback behavior for all three databases.
- Added focused `DataTextParser` tests and whitespace-tolerant parser coverage for item, recipe, and buildable rows.
- Added `Docs/Systems/DataLoadingConsolidation.md`.

Remaining deferred items:

- Duplicate ID validation remains a future low-risk improvement.
- Biome layouts, drop tables, and objectives remain code-defined until their own scoped passes.
- Do not begin terrain, procedural generation, renderer rewrite, real UI/HUD, or new gameplay systems yet.

## Stabilisation Pass 5 Result

Completed after this review:

- Added `Game/Data/Buildables/buildables.txt` as a pipe-delimited buildable definition file mirroring the existing starter buildables.
- Added a tiny dependency-free buildable parser with validation for field count, required fields, primitive names, cost syntax, positive costs, scale triples, positive scale values, placement distance, and placement radius.
- Added `BuildableDatabase::LoadFromText`, `LoadFromFile`, and `CreateFromFileOrFallback`.
- Updated runtime buildable database construction to load buildable definitions from data first and fall back to hardcoded definitions with a warning.
- Preserved stable buildable IDs, placement costs, preview scale, placed scale, placement distance, placement radius, and placed buildable instance ID behavior.
- Added buildable parser coverage to the building/domain tests while preserving existing inventory, crafting, gathering, objective, build placement, and save/load behavior.
- Added `Docs/Systems/DataLoadingBuildables.md`.

Remaining deferred items:

- Keep biome layouts, drops, and objectives code-defined until their own scoped passes.
- Do not begin terrain, procedural generation, renderer rewrite, real UI/HUD, or new gameplay systems yet.
