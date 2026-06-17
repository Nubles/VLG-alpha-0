# Building Placement

Milestone 6 adds a minimal build placement prototype using the existing inventory and crafted items. It is a debug system only: buildables are primitive scene objects, placement is deterministic, and no construction simulation exists yet.

## Architecture

- Building gameplay code lives in `Game/Source/Building`.
- `BuildableDefinition` stores a stable ID, display name, item costs, primitive type, preview scale, placed scale, placement distance, placement radius, and category.
- `BuildableDatabase` owns the starter buildables in code for now.
- `BuildPlacementController` owns build mode, selected buildable, preview transform, yaw rotation, placement validation, and all-or-nothing cost consumption.
- Placed buildables are recorded in Game code and mirrored into the existing `Scene` as persistent debug primitives.
- Engine changes are generic: scene objects can be updated for preview rendering, and debug cubes now respect yaw rotation.

## Starter Buildables

- `camp_marker`: costs camp_bundle x1. A small marker proving crafted items can become world objects.
- `workbench_stub`: costs workbench_kit x1. A visual-only placeholder for a later station milestone.
- `simple_wall`: costs wood x3 and fiber x1. A thin upright cube proving direct resource-cost building.

## Controls

- B: toggle build mode
- 1: select Camp Marker
- 2: select Workbench Stub
- 3: select Simple Wall
- Q: rotate preview left
- R: rotate preview right
- Enter: place selected buildable
- Backspace: cancel build mode

Existing movement, interaction, gathering, item grant, crafting, and Escape exit controls are unchanged.

## Preview Behavior

The preview appears a fixed distance in front of the camera for the selected buildable and follows the player as the camera moves or looks around. Rotation adjusts the preview yaw. The preview uses the existing cube primitive with a distinct debug colour.

## Placement Validation

Placement is intentionally simple:

- build mode must be active
- a known buildable must be selected
- preview distance must be at least 1.0 unit from the player
- preview distance must be no more than 8.0 units from the player
- the preview must not overlap an existing placed buildable, using simple radius checks

No physics, terrain raycast, snapping, or structural validation is implemented.

## Inventory Cost Behavior

Placement checks required costs before consuming anything. If validation fails or costs are missing, inventory is unchanged. On success, the buildable cost is removed and a persistent scene object is added.

## Not Implemented Yet

- snapping
- structural integrity
- decay
- comfort
- functional crafting stations
- workbench recipe requirements
- undo/demolish
- final UI
- terrain-aware placement
- physics
- combat, enemies, AI, procedural worlds, save/load, or biomes
