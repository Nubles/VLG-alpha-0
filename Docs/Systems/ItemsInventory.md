# Items + Inventory

Milestone 3 adds a small data-driven item and inventory foundation. It does not add resource gathering, crafting, building, combat, enemies, terrain, procedural generation, save/load, biomes, final UI, or equipment behavior.

## Item Definitions

Item definitions live in Game code for now and are bootstrapped by `ItemDatabase::CreateStarterDatabase()`. Each definition has:

- Stable ID.
- Display name.
- Description.
- Category.
- Max stack size.
- Resource tier.

The starter database is hardcoded to avoid adding a parser or dependency before the data format stabilizes.

## Starter Items

- `wood`: resource, max stack 50.
- `stone`: resource, max stack 50.
- `fiber`: resource, max stack 50.
- `raw_food`: food, max stack 20.
- `primitive_tool`: tool, max stack 1.

## Inventory Behavior

`Inventory` has a fixed slot count. Each slot stores an `ItemStack` with an item ID and quantity.

Adding items:

1. Finds the item definition by stable ID.
2. Merges into existing stacks first.
3. Uses empty slots when existing stacks are full.
4. Respects the item max stack size.
5. Returns leftover quantity if the inventory cannot fit everything.

Removing items checks the total available quantity first. If there is not enough quantity, removal fails and the inventory is unchanged.

## Hotbar Behavior

`Hotbar` has a fixed size of eight entries in the current game setup. Entries reference inventory slot indices. The hotbar supports assigning inventory slots, selecting a specific index, selecting next, and selecting previous.

There is no final hotbar UI yet.

## Debug Grant Keys

Temporary debug keys grant starter items:

- `F1`: grants wood.
- `F2`: grants stone.
- `F3`: grants fiber.
- `F4`: grants raw food.
- `F5`: grants primitive tool.

The window title shows a compact inventory summary and the most recent grant message.

## Build

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

## Test

```powershell
.\build\Debug\EngineCoreTests.exe
.\build\Debug\GameCoreTests.exe
```

## Manual Checklist

1. App opens.
2. Grid and cube still render.
3. Player movement still works.
4. Ancient Stone interaction still works.
5. Pressing debug item grant keys logs or shows item changes.
6. Inventory stacking works according to tests.
7. Escape exits cleanly.

## Intentionally Not Implemented Yet

- Resource gathering.
- Crafting or recipes.
- Building placement.
- Equipment, armor, combat, or enemies.
- Inventory screen, drag/drop UI, or final hotbar UI.
- Save/load.
- JSON/TOML item loading.

