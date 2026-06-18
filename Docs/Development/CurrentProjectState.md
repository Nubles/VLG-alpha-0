# Current Project State

Realmbound Wilds currently has a playable debug vertical slice built on a custom C++20 engine/game codebase.

## Completed Milestones

- Milestone 0: custom engine bootstrap with CMake, Engine/Game separation, Win32 windowing, main loop, delta time, logging, input polling, blank screen, clean shutdown, and debug overlay placeholder.
- Milestone 1: basic 3D sandbox with fixed-function OpenGL, debug grid, cube primitives, math types, camera, scene objects, and debug camera movement.
- Milestone 2: player controller and interaction with health, stamina, sprinting, camera ray targeting, and an interactable test object.
- Milestone 3: item definitions and inventory foundation.
- Milestone 4: resource gathering.
- Milestone 5: crafting.
- Milestone 6: building placement.
- Milestone 7: combat and basic enemy AI.
- Milestone 8: first handcrafted biome test area, Mistwood Hollow.
- Milestone 9: Realm Fracture objective and progression.
- Milestone 10: save/load prototype.
- Milestone 11: vertical slice polish.

## Completed Stabilisation Passes

- Pass 1: `SandboxWorldState`, stable gatherable IDs, placed buildable instance IDs, gatherable depletion persistence, and camera yaw/pitch persistence.
- Pass 2: game tests split by domain, manual QA doc, and improved Realm Fracture requirement feedback.
- Pass 3: item definitions load from `Game/Data/Items/items.txt`.
- Pass 4: recipe definitions load from `Game/Data/Recipes/recipes.txt`.
- Pass 5: buildable definitions load from `Game/Data/Buildables/buildables.txt`.
- Pass 6: item, recipe, and buildable text-loading helpers consolidated in `Game/Source/Data/DataTextParser`.
- Pass 7: developer onboarding, README, CI guide, and current-state docs.

## Current Data-Loaded Definitions

Items:

- `wood`
- `stone`
- `fiber`
- `raw_food`
- `primitive_tool`
- `camp_bundle`
- `simple_meal`
- `workbench_kit`
- `realm_anchor`

Recipes:

- `primitive_tool`
- `camp_bundle`
- `simple_meal`
- `workbench_kit`
- `realm_anchor`

Buildables:

- `camp_marker`
- `workbench_stub`
- `simple_wall`

## Still Code-Defined

- Mistwood Hollow biome layout
- gatherable drop tables
- objective definitions
- enemy definitions

These should remain code-defined until their own scoped pass. Do not mix them into unrelated work.

## Known Limitations

- no real HUD
- no mouse-look
- no terrain or procedural generation
- no models, textures, or authored art assets
- no audio or VFX
- no multiplayer
- one quick-save slot
- debug title/messages are doing most tester feedback work

## Recommended Next Decision

The next major decision should be one of:

- UI/HUD foundation, so testers get clearer on-screen feedback, or
- another deliberate architecture pass, so the current vertical slice stays maintainable before larger content work.

Do not jump straight into terrain or procedural generation from here.
