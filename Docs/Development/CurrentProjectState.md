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
- Milestone 12: UI/HUD foundation with dependency-free overlay bars and debug panels.
- Milestone 13: dependency-free debug text overlay using built-in rectangle glyphs.
- Milestone 14: mouse-look toggle and input polish foundation with arrow-key fallback.
- Milestone 15: contextual HUD prompts and vertical-slice next-step guidance.

## Completed Stabilisation Passes

- Pass 1: `SandboxWorldState`, stable gatherable IDs, placed buildable instance IDs, gatherable depletion persistence, and camera yaw/pitch persistence.
- Pass 2: game tests split by domain, manual QA doc, and improved Realm Fracture requirement feedback.
- Pass 3: item definitions load from `Game/Data/Items/items.txt`.
- Pass 4: recipe definitions load from `Game/Data/Recipes/recipes.txt`.
- Pass 5: buildable definitions load from `Game/Data/Buildables/buildables.txt`.
- Pass 6: item, recipe, and buildable text-loading helpers consolidated in `Game/Source/Data/DataTextParser`.
- Pass 7: developer onboarding, README, CI guide, and current-state docs.
- Pass 8: vertical slice release-candidate QA, stability inspection, prompt-priority fix, and RC checklist.
- Pass 9: attempted local Windows release-candidate verification; local configure did not complete from this shell, so interactive playthrough remains required.
- Pass 10: GitHub Actions playtest artifact packaging for manual RC verification when local configure/build is unavailable.
- Pass 11: downloaded and launched the CI playtest artifact; basic debug controls, save, load, and clean shutdown were verified through title/debug output, but full visual/manual RC playthrough remains required.
- Pass 12: launched the latest CI artifact in a visible window and confirmed scene/HUD/prompt/guidance rendering by screenshot; full hands-on RC certification is still not complete.

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

- no full UI system or real font-rendered HUD
- no terrain or procedural generation
- no models, textures, or authored art assets
- no audio or VFX
- no multiplayer
- no settings menu, key rebinding UI, controller support, or mouse-driven UI interaction
- one quick-save slot
- debug title/messages still mirror detailed tester feedback text
- HUD prompt/guidance is specific to the current Mistwood Hollow vertical slice
- CI artifact launch is verified, but rendered HUD inspection, target aiming, combat, gatherable depletion, and Realm Fracture completion still need a true hands-on Windows playthrough
- the latest visual artifact attempt confirms rendering, but mouse-look feel, complete gathering/crafting/build/combat/fracture flow, save/load restoration, and cursor observation still need direct human certification

## Recommended Next Decision

The next major decision should be one of:

- a deliberate architecture pass, so the current vertical slice stays maintainable before larger content work, or
- a scoped text/UI planning pass before adding real fonts, inventory grids, mouse interaction, or final HUD behavior.

Do not jump straight into terrain or procedural generation from here.
