# First Biome Test Area

Milestone 8 adds the first handcrafted biome test area: Mistwood Hollow. It uses existing primitive scene objects and gameplay systems to create a small, intentional debug wilderness space.

## Biome Definition Structure

Biome definitions live in `Game/Source/Biomes`.

`BiomeDefinition` includes:

- stable biome ID
- display name
- description
- difficulty tier
- debug clear colour
- ambience text
- resource theme tags
- enemy theme tags
- landmark names
- optional future guardian ID
- optional future weather profile ID

`BiomeDatabase` currently creates the starter biome definition in code.

## Mistwood Hollow

ID: `mistwood_hollow`

Mistwood Hollow is a quiet, mist-heavy woodland where unstable realm energy leaks through old stones and broken roots.

Theme:

- resources: wood, stone, fiber, raw_food
- enemy: Realm Wisp
- landmarks: Old Root Circle, Broken Waystone, Realm Fracture
- difficulty tier: 1

## Handcrafted Layout Contents

`BiomeLayoutBuilder::BuildMistwoodHollow` creates:

- player spawn near the southern side of the test area
- debug grid
- Old Root Circle landmark
- Broken Waystone landmark
- Ancient Stone lore interactable
- Realm Fracture placeholder interactable
- Branch Pile cluster near spawn
- Loose Stone near the rock/waystone side
- Fiber Plant near old roots
- Cracked Rock near the edge
- one Realm Wisp near the Realm Fracture

The area leaves open space for gathering, crafting, building placement, and combat.

## Realm Fracture Placeholder

The Realm Fracture is a visible primitive object and an interactable. Interacting with it logs:

`The fracture shimmers, but it is not stable enough to enter.`

It does not teleport the player, unlock progression, spawn enemies, or change the world.

## Existing Systems Preserved

Mistwood Hollow reuses:

- existing `SceneObject` debug rendering
- existing `Interactable`
- existing `GatherableNode`
- existing Realm Wisp enemy
- existing inventory, crafting, building, combat, and debug controls

## Not Implemented Yet

- procedural world generation
- terrain chunks
- real terrain mesh
- save/load
- boss systems
- shrine progression completion
- advanced AI or pathfinding
- multiplayer
- final UI
- mouse-look
- textures, models, asset loading, audio, or VFX polish

## Manual Test Checklist

1. App opens.
2. Debug title shows Mistwood Hollow.
3. Player spawns into a more intentional test area.
4. Resource nodes are visible and gatherable.
5. Ancient Stone interaction still works.
6. Realm Fracture is visible.
7. Interacting with Realm Fracture shows/logs the placeholder message.
8. Realm Wisp is visible near the fracture.
9. Realm Wisp detects, chases, attacks, dies, and drops loot.
10. Crafting still works.
11. Building placement still works in open space.
12. Existing debug grant/craft/build controls still work.
13. Escape exits cleanly.
