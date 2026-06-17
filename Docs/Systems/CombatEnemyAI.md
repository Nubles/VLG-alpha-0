# Combat And Enemy AI

Milestone 7 adds a small debug combat and enemy AI prototype. It proves damage, a player melee attack, enemy chase/attack behavior, enemy death, and deterministic loot drops without adding equipment, animation, physics, or pathfinding.

## Combat Architecture

- Combat code lives in `Game/Source/Combat`.
- `Damageable` owns max health, current health, damage application, and alive/dead checks.
- `DamageEvent` carries damage amount and source text.
- `PlayerCombat` owns the temporary melee attack cooldown and performs a simple camera-forward range/aim check.

## Player Attack Behavior

- C performs a debug melee attack.
- Attack damage is 25.
- Attack range is 3 units.
- Attack cooldown is 0.5 seconds.
- The attack uses the current camera position and forward direction.
- No weapon, equipment slot, stamina cost, animation, or mouse input is used yet.

## Player Damage

`PlayerVitals` now supports `ApplyDamage`, `Heal`, and `IsAlive`. Enemy attacks reduce player health. When health reaches zero, the debug message reports `Player down`. Respawn is not implemented.

## Enemy AI Architecture

- Enemy AI code lives in `Game/Source/AI`.
- `EnemyBrain` evaluates the state from health and distance.
- `EnemyAgent` owns movement, attack cooldown, health, drop table, transform, and state.

## Realm Wisp

The sandbox spawns one Realm Wisp as a cube primitive.

- health: 50
- detection range: 8 units
- attack range: 1.5 units
- attack damage: 10
- attack cooldown: 1 second
- movement speed: 2 units per second
- states: Idle, Chase, Attack, Dead

The wisp starts idle, chases when the player is in detection range, attacks when close enough, and stops updating after death.

## Loot Drops

On death, the Realm Wisp rolls its deterministic drop table and adds loot to the existing inventory. Current starter drop is `fiber x1`. If inventory cannot fit all loot, leftover quantity is reported in the debug message.

## Debug Controls

- C: melee attack
- F10: reset Realm Wisp

Existing movement, interaction, gathering, crafting, building, and Escape controls are unchanged.

## Not Implemented Yet

- advanced combat
- ranged weapons
- armor or equipment slots
- status effects
- animations
- navmesh or pathfinding
- physics
- boss systems
- base raids
- multiplayer
- audio or VFX polish
- terrain, procedural worlds, save/load, biomes, final UI, or mouse-look
