# Resource Gathering

Milestone 4 connects simple interactable world resource nodes to the existing item database and inventory. It does not add crafting, recipes, building, combat, enemies, AI, terrain generation, procedural worlds, save/load, biomes, final UI, mouse look, a physics engine, equipment slots, or tool durability.

## Gatherable Node Structure

`GatherableNode` lives in Game code and contains:

- Name.
- Transform/position.
- Interaction radius.
- Remaining gather count.
- Drop table.
- Optional required tool item ID.
- Debug message.
- Depleted state derived from remaining gather count.

The node uses the same simple camera ray style as the existing interaction prototype. There is no physics scene or collider system yet.

## Drop Table Behavior

`DropTable` stores deterministic drop entries with:

- Item ID.
- Minimum quantity.
- Maximum quantity.

For this milestone, drops are deterministic and use the maximum quantity. Chance, rarity, random rolls, and weighted drops are intentionally deferred.

## Starter Resource Nodes

- `Branch Pile`: gives wood, no tool required.
- `Loose Stone`: gives stone, no tool required.
- `Fiber Plant`: gives fiber, no tool required.
- `Cracked Rock`: gives stone, requires `primitive_tool`.

`Ancient Stone` remains as a non-resource lore interactable.

## Tool Requirements

Tool requirements only check whether the inventory contains the required item ID. `Cracked Rock` requires `primitive_tool`.

There are no equipment slots, tool durability, tool consumption, damage, or animation hooks yet.

## Debug And Manual Controls

- `WASD`: move.
- Arrow keys: look.
- `Shift`: sprint.
- `E`: interact or gather current target.
- `F1`: grant wood.
- `F2`: grant stone.
- `F3`: grant fiber.
- `F4`: grant raw food.
- `F5`: grant primitive tool.
- `Escape`: exit.

The debug title shows the current target, compact inventory summary, and recent grant/gather messages.

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
4. Ancient Stone non-resource interaction still works.
5. Looking at Branch Pile shows it as target.
6. Pressing `E` gathers wood into inventory.
7. Looking at Loose Stone gathers stone.
8. Looking at Fiber Plant gathers fiber.
9. Cracked Rock refuses gathering without primitive_tool.
10. Pressing `F5` grants primitive_tool.
11. Cracked Rock can then be gathered.
12. Depleted nodes stop giving resources after their gather count reaches zero.
13. `F1`-`F5` debug item grants still work.
14. Escape exits cleanly.

## Intentionally Not Implemented Yet

- Crafting or recipes.
- Building placement.
- Combat, enemies, or AI.
- Terrain generation or procedural worlds.
- Save/load.
- Biomes.
- Equipment slots or tool durability.
- Physics-engine raycasts.
- Respawning resources.

