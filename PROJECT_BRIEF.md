# Realmbound Wilds Project Brief

## Game Vision

Realmbound Wilds is a solo-developer-friendly survival crafting adventure set in a mysterious fantasy wilderness where unstable realms bleed into each other. The player explores dangerous regions, gathers resources, builds shelters and outposts, crafts better equipment, discovers ancient ruins, and defeats guardians to stabilize or claim parts of the world.

The project direction is to build a small project-specific custom C++ engine first, then build Realmbound Wilds on top of it. The engine should support only what this game needs: a focused survival crafting vertical slice, debug tools, data-driven definitions, and room to expand without trying to compete with general-purpose engines.

The goal is to build a focused vertical slice first, then expand through modular systems. The prototype should prove the core loop before adding advanced procedural generation, multiplayer, complex factions, or large content libraries.

## Core Pillars

### Explore

The world should reward curiosity with resources, ruins, caves, shrines, enemy camps, rare events, boss clues, and realm fractures. Exploration should feel atmospheric and purposeful rather than empty.

### Survive

Survival should encourage planning without becoming tedious. Health, stamina, food buffs, weather exposure, danger level, and preparation should matter, but the player should not be punished by constant upkeep.

### Craft

Resources should unlock tools, weapons, armor, stations, building pieces, food, potions, and exploration equipment. Crafting should be readable, expandable, and strongly tied to discovery.

### Build

Building should support practical bases, outposts, crafting halls, storage, farms, defenses, waystones, and decorative structures. The first version should be simple but structured so snapping, stability, upgrades, and comfort can be added later.

### Progress

Progression should come from discoveries, crafted equipment, defeated guardians, biome materials, upgraded stations, and stabilized regions.

### Prepare for Danger

Bosses, storms, raids, caves, ruins, and hostile camps should reward players who bring the right tools, food, equipment, and resistances.

### Expand Modularly

Items, recipes, enemies, biomes, bosses, structures, status effects, events, and progression gates should be data-driven where practical and isolated enough to expand without rewriting core systems.

## Core Loop

1. Spawn in a mysterious wilderness.
2. Gather basic resources.
3. Craft primitive tools.
4. Build a first shelter and workbench.
5. Explore nearby wilderness.
6. Discover ruins, resources, enemies, and realm fractures.
7. Unlock new recipes.
8. Improve gear and base.
9. Find clues to a guardian or shrine objective.
10. Prepare with food, armor, tools, and weapons.
11. Defeat the objective.
12. Unlock the next material, biome, or realm interaction.
13. Repeat with new mechanics and greater danger.

## Unique Twist

The world is made of unstable realms that bleed into one another. Realm fractures create temporary pocket zones, strange resource opportunities, environmental hazards, and ancient gates. Defeating guardians stabilizes or corrupts regions, letting the player claim outposts, improve farms and mines, and unlock realm-energy upgrades.

This identity should shape future systems:

- Realm fractures as exploration events.
- Claimable outposts as long-term regional goals.
- Guardians that change regional stability.
- Procedural ruins that reveal realm history.
- Temporary mini-realms with rare resources.
- Base upgrades tied to realm energy.

## What We Are Not Copying

Realmbound Wilds may use broad genre inspiration from survival crafting games, but it must not copy Valheim's names, assets, exact mechanics, lore, enemy designs, UI, progression structure, or art style.

The project should avoid:

- Norse mythology as the defining premise.
- Direct equivalents of Valheim bosses, biomes, enemies, items, or materials.
- The exact same low-poly visual identity.
- Matching UI layout, terminology, or progression beats.
- Recreating mechanics one-for-one when a distinct alternative can serve the same design purpose.

## Initial Vertical Slice

The first playable slice should include one small world area and enough systems to prove the loop:

- A small custom engine runtime with a window, main loop, input, rendering, scene objects, debug overlay, and data loading.
- Basic first-person or third-person player controller.
- Movement, camera, health, and stamina.
- Simple interaction trace or raycast.
- Basic gatherable resources.
- Inventory and hotbar.
- Item data and recipe data.
- Simple crafting at a workbench.
- Basic building placement with preview, rotation, cost, and cancel/place controls.
- One basic enemy with idle, chase, attack, and loot drop behavior.
- Basic day/night cycle.
- One shrine or mini-boss objective that unlocks a first progression flag.
- Basic save/load if feasible within the milestone.
- Debug tools for granting items, toggling invulnerability, spawning test objects, and resetting progress.

## Long-Term Features

- Multiple biome tiers with unique resources, enemies, weather, and mechanics.
- Guardians tied to realm stability and regional progression.
- More complete building with snapping, upgrades, decay, defenses, and comfort.
- Expanded inventory, equipment, armor, resistances, and status effects.
- Farms, mines, outposts, and realm-energy infrastructure.
- World events such as storms, raids, wandering threats, and rare resource windows.
- Procedural ruins, caves, shrines, and temporary pocket realms.
- Map discovery and exploration notes.
- Future co-op-ready architecture with authoritative gameplay state boundaries.
