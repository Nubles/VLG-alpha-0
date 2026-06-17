# Shrine Objective Progression

Milestone 9 adds a small progression objective at the Mistwood Hollow Realm Fracture. It proves named flags, objective requirements, completion behavior, a visual/debug state change, and a simple recipe unlock.

## Progression Flags

Progression code lives in `Game/Source/Progression`.

`ProgressionState` supports:

- setting a named flag
- checking whether a flag is active
- clearing all flags for tests/debug reset
- compact debug summary output

Starter flags:

- `mistwood_fracture_discovered`
- `mistwood_shrine_completed`
- `mistwood_fracture_stabilized`
- `recipe_realm_anchor_unlocked`

## Mistwood Fracture Objective

Objective code lives in `Game/Source/Objectives`.

Objective ID: `mistwood_fracture_shrine`

Display name: `Stabilise the Mistwood Fracture`

The player stabilises the unstable fracture using gathered and crafted items. Interacting with the Realm Fracture always marks it discovered. If requirements are missing, the objective reports the missing item and consumes nothing.

## Objective Requirements

Required items:

- camp_bundle x1, consumed
- primitive_tool x1, required but not consumed
- fiber x3, consumed

On completion, the objective sets:

- `mistwood_shrine_completed`
- `mistwood_fracture_stabilized`
- `recipe_realm_anchor_unlocked`

The Realm Fracture message changes to:

`The fracture steadies into a calm, silver-blue shimmer.`

The primitive visual also changes to a calmer silver-blue debug colour and adjusted scale.

## Recipe Unlock Behavior

`RecipeDefinition` already had an optional unlock field. `CraftingService` now checks that field when a `ProgressionState` is provided. Recipes with no unlock flag remain available.

The `realm_anchor` recipe requires:

- `recipe_realm_anchor_unlocked`
- workbench_kit x1
- fiber x5
- stone x5

If the flag is missing, crafting fails clearly and consumes nothing.

## New Item And Recipe

New item:

- `realm_anchor`
- category: Tool
- max stack: 1
- description: A crude anchor of stabilized realm energy. It has no world function yet.

New recipe:

- `realm_anchor`
- output: realm_anchor x1
- ingredients: workbench_kit x1, fiber x5, stone x5
- locked until `recipe_realm_anchor_unlocked` is set

## Debug Controls

- F11: craft realm_anchor
- F12: reset progression/objective state

Existing controls for movement, interaction, gathering, crafting, building, combat, and Escape exit are unchanged.

## Not Implemented Yet

- full boss system
- complex quest system
- procedural generation
- save/load
- advanced AI or pathfinding
- multiplayer
- final UI
- real teleportation between realms
- full recipe unlock tree
- audio or VFX polish

## Manual Test Checklist

1. App opens.
2. Mistwood Hollow loads.
3. Existing movement, gathering, crafting, building, and combat still work.
4. Realm Fracture is visible and targetable.
5. Interacting before requirements reports a missing item.
6. Gather/craft or debug grant required items.
7. Interacting completes the objective.
8. camp_bundle and fiber are consumed.
9. primitive_tool remains in inventory.
10. Realm Fracture message and visual change.
11. Interacting again says it is already stabilised.
12. F11 attempts realm_anchor crafting.
13. realm_anchor crafting fails before unlock and succeeds after unlock.
14. F12 resets progression/objective state.
15. Escape exits cleanly.
