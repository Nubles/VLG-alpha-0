# Realmbound Wilds Roadmap

## Milestone 0: Custom Engine Bootstrap

Goal: create the smallest custom C++ engine shell that can build and run.

Deliverables:

- CMake project.
- Engine/Game folder separation.
- Window creation.
- Main loop.
- Logging.
- Time and delta-time.
- Input polling.
- Blank screen.
- Debug overlay placeholder.
- Documentation.

Exit criteria:

- Project configures and builds.
- The application opens a window.
- The main loop runs with stable delta-time.
- Input polling detects at least quit/close.
- A blank screen clears consistently.
- Developer can follow docs to build and run from a clean checkout.

## Milestone 1: Basic 3D Sandbox

Goal: prove the rendering and scene pipeline with primitive 3D objects.

Deliverables:

- Renderer draws primitive shapes.
- Camera movement.
- Basic scene objects.
- Transform system.
- Debug grid.
- Simple test scene.

Exit criteria:

- A test scene renders primitive objects and a debug grid.
- Camera can move through the scene.
- Scene objects have transforms that can be inspected or logged.

## Milestone 2: Player Controller + Interaction

Goal: make the game controllable from a player perspective and establish the interaction foundation.

Deliverables:

- First-person or third-person controller.
- Health and stamina values.
- Sprint stamina drain.
- Stamina regeneration after a short delay.
- Interaction raycast.
- Basic interactable interface.
- Debug readout for health, stamina, and hit interactable.

Exit criteria:

- Player can move and look around the sandbox.
- Sprinting drains stamina and recovery works.
- Interaction raycast reports the name of a hit interactable object.

## Milestone 3: Data-Driven Items + Inventory

Goal: establish item definitions and the first inventory model.

Deliverables:

- Item definitions loaded from data files.
- Item stacks.
- Inventory slots.
- Hotbar.
- Debug item granting.

Exit criteria:

- Item definitions load at startup.
- Debug tools can grant items.
- Stack limits and empty slots behave predictably.
- Hotbar selection works.

## Milestone 4: Resource Gathering

Goal: let the player gather resources from simple world nodes.

Deliverables:

- Gatherable resource nodes.
- Drop tables.
- Tool requirements.
- Add gathered items to inventory.

Exit criteria:

- Player can gather from a test node.
- Drops are driven by data.
- Tool requirements can allow or block gathering.

## Milestone 5: Crafting

Goal: let the player convert resources into useful items.

Deliverables:

- Recipe definitions.
- Ingredient checks.
- Crafting service.
- Workbench or station requirement.

Exit criteria:

- Recipes load from data.
- Crafting consumes ingredients and produces output.
- Station requirements can block or allow recipes.
- Failures produce clear debug output.

## Milestone 6: Building Placement

Goal: let the player place simple build pieces in the world.

Deliverables:

- Placeable build pieces.
- Preview ghost.
- Rotation.
- Cost checks.
- Placement validation.

Exit criteria:

- Player can preview, rotate, place, and cancel a build piece.
- Invalid placement is blocked.
- Costs are consumed only on successful placement.

## Milestone 7: Combat + Enemy AI

Goal: create one basic threat and a minimal combat loop.

Deliverables:

- Damage system.
- Basic enemy state machine.
- Enemy attacks.
- Loot drops.

Exit criteria:

- Player can damage and defeat a basic enemy.
- Enemy can detect, chase, and attack the player.
- Enemy drops loot through the same item/inventory path used by resources.

## Milestone 8: First Biome Test Area

Goal: assemble the first small handcrafted wilderness zone.

Deliverables:

- Small handcrafted wilderness zone.
- Resources.
- One enemy.
- Shrine.
- Ambience placeholders.
- Realm fracture placeholder.

Exit criteria:

- The area supports movement, interaction, gathering, crafting, building, and combat.
- Realm fracture placeholder gives the zone a distinct identity.
- The test area is small enough to iterate quickly.

## Milestone 9: Shrine/Boss Objective + Progression

Goal: create a focused progression objective.

Deliverables:

- Shrine or mini-boss encounter.
- Progression flag.
- Recipe or region unlock.
- Realm stabilization placeholder.

Exit criteria:

- Player can discover and complete the objective.
- Completion sets a progression flag.
- At least one recipe, station, material, or region gate unlocks afterward.

## Milestone 10: Save/Load

Goal: persist the first vertical slice state.

Deliverables:

- Player state.
- Inventory.
- Placed structures.
- Gathered resources.
- Progression flags.

Exit criteria:

- Player can quit and reload with vertical slice state preserved.
- Save reset works for testing.
- Save data uses stable IDs for definitions.

## Milestone 11: Vertical Slice Polish

Goal: make the first complete loop coherent, testable, and presentable.

Deliverables:

- Basic UI.
- Onboarding prompts.
- Audio and VFX placeholders.
- Performance pass.
- Bug pass.
- Test checklist.

Exit criteria:

- A new tester can play from spawn to shrine completion.
- The application has no blocking errors.
- Debug tools make iteration fast.
- The project is ready for the next expansion decision.

## Milestone 12: UI/HUD Foundation

Goal: add the first minimal UI/HUD path without introducing UI libraries, font dependencies, or a renderer rewrite.

Deliverables:

- Engine-side generic 2D overlay primitives.
- Game-side HUD state snapshot.
- Health and stamina indicators.
- Target, objective, message, enemy, and build mode panel placeholders.
- Pure HUD formatting/layout tests.
- Documentation.

Exit criteria:

- HUD overlay renders on top of the existing 3D sandbox.
- Health and stamina indicators reflect current player state.
- Detailed text remains available through the existing title/debug output.
- Existing vertical-slice controls and flow remain unchanged.
- CI remains green.

## Milestone 13: Debug Text Overlay Foundation

Goal: add readable dependency-free debug text to the existing HUD without adding a full UI system or font dependency.

Deliverables:

- Built-in monospaced rectangle glyph text renderer.
- Generic engine-side text-to-overlay-rect generation.
- HUD text labels for health, stamina, target, objective, enemy, build mode, messages, and save/load feedback.
- Pure tests for glyph support, sanitization, max character limits, and HUD text generation.
- Documentation.

Exit criteria:

- Debug text renders in-game through the existing overlay path.
- Window title and console/debug logging still work.
- No external font library, ImGui, DirectWrite, asset loading, or UI framework is added.
- Existing vertical-slice controls and flow remain unchanged.
- CI remains green.

## Milestone 14: Mouse Look + Input Polish Foundation

Goal: add a small mouse-look foundation without rewriting input or adding a settings system.

Deliverables:

- Mouse position and per-frame delta in `InputState`.
- Optional mouse capture flag.
- Win32 cursor hide, clip, and recenter behavior while mouse look is enabled.
- `M` toggle for mouse look.
- Mouse yaw/pitch camera control in `PlayerController`.
- Arrow-key look fallback preserved.
- HUD/title/help text updated with mouse-look state/control.
- Pure tests for mouse input and player look behavior.
- Documentation.

Exit criteria:

- App opens with mouse look off by default.
- Arrow-key look still works.
- Pressing `M` toggles mouse look.
- Mouse delta changes camera yaw/pitch while enabled.
- Pitch remains clamped.
- Existing vertical-slice controls and flow remain unchanged.
- CI remains green.

## Milestone 15: Contextual Prompts + Vertical Slice Guidance

Goal: help new testers understand the current vertical slice using lightweight HUD text without adding a quest or tutorial system.

Deliverables:

- Game-side contextual prompt builder.
- Game-side vertical-slice next-step guide.
- HUD prompt and next-step text lines.
- Help/manual QA documentation updates.
- Pure tests for prompt and guide ordering.

Exit criteria:

- Prompt line reflects target, missing tool, Realm Fracture, enemy, or build context.
- Next-step line advances through the existing vertical-slice flow.
- No new gameplay content or persistent quest/tutorial system is added.
- Existing controls and flow remain unchanged.
- CI remains green.

## Post-Vertical-Slice Stabilisation

Milestones 0-15 have produced the first playable debug vertical slice with HUD text, mouse-look support, contextual prompts, and next-step guidance. Before adding another major gameplay milestone, the project should stabilise the current slice so future systems do not pile onto fragile integration code.

Recommended cleanup order:

1. Extract current vertical-slice runtime state and save/load capture/apply logic out of `SandboxGame`.
2. Add stable IDs to gatherable nodes and placed buildable instances.
3. Persist gatherable depletion in the save/load prototype.
4. Split or reorganise `GameCoreTests.cpp` into clearer domain-focused test sections/files.
5. Update developer/manual QA documentation around the full happy path.
6. Add camera yaw/pitch to save/load.
7. Improve objective requirement feedback.
8. Complete a minimal data-loading pass for item definitions, then recipes, then buildables.

Completed stabilisation notes:

- Item definitions now load from `Game/Data/Items/items.txt` with a hardcoded fallback.
- Recipe definitions now load from `Game/Data/Recipes/recipes.txt` with a hardcoded fallback.
- Buildable definitions now load from `Game/Data/Buildables/buildables.txt` with a hardcoded fallback.
- Item, recipe, and buildable data loading now share small parser helpers in `Game/Source/Data`.
- Stabilisation Pass 8 performed release-candidate QA inspection and fixed target prompt priority when a nearby enemy is relevant.
- Biome layout, drop table, and objective data loading remain deferred.

Primary next direction: review the release-candidate debug vertical slice, then choose the next major phase deliberately.

Next milestone options after stabilisation:

- Renderer/debug visual improvements.
- Proper UI/HUD foundation.
- Terrain/world foundation.
- Second biome/progression chain.
- Enemy/combat expansion.

Before choosing one of those options, keep onboarding, README, CI, and current-state documentation accurate. Item, recipe, and buildable data loading are complete. Shared parser helpers for those data-loading paths are complete. Biome layout, drop table, and objective data loading remain deferred until their own scoped pass.
