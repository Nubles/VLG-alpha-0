# Crafting

Milestone 5 adds a small game-side crafting prototype. It consumes items from the existing inventory and produces a crafted item only when the whole craft can succeed.

## Architecture

- Crafting code lives in `Game/Source/Crafting`.
- `RecipeDefinition` stores stable recipe data: recipe ID, display name, output item ID, output quantity, ingredients, optional station ID, and optional unlock ID.
- `RecipeDatabase` owns the starter recipes in code for now.
- `CraftingService` performs pure inventory checks, simulates the craft on an inventory copy, then applies the ingredient removal and output add only on success.
- The engine does not know about recipes, items, inventory, or crafting.

## Starter Items

- `camp_bundle`
- `simple_meal`
- `workbench_kit`

These are inventory items only. Placement, station behavior, food effects, and equipment behavior are intentionally not implemented yet.

## Starter Recipes

- `primitive_tool`: wood x1, stone x1, fiber x1
- `camp_bundle`: wood x5, fiber x3
- `simple_meal`: raw_food x2, fiber x1
- `workbench_kit`: wood x10, stone x4

All starter recipes have no required station. The `requiredStationId` field exists for a later milestone.

## Debug Controls

- F1: grant wood
- F2: grant stone
- F3: grant fiber
- F4: grant raw_food
- F5: grant primitive_tool
- F6: craft primitive_tool
- F7: craft camp_bundle
- F8: craft simple_meal
- F9: craft workbench_kit

The window title shows health, stamina, current target, inventory summary, and the latest grant, gather, or craft message.

## Failure Behavior

- Unknown recipe fails with no inventory changes.
- Missing ingredients fail with no inventory changes.
- Missing output item definitions fail with no inventory changes.
- Inventory capacity failure fails with no inventory changes.

Crafting is intentionally all-or-nothing for now.

## Not Implemented Yet

- Building placement
- Workbench/world crafting stations
- Recipe unlock progression
- Equipment or tool durability
- Final UI
- Combat, enemies, AI, terrain generation, procedural worlds, save/load, or biomes
