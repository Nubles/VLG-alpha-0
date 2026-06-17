# Vertical Slice Stabilisation Pass 1

This pass reduces integration risk without adding new gameplay. It focuses on runtime state identity and save/load correctness for the current vertical slice.

## What Was Stabilised

- Gatherable nodes now have deterministic stable IDs.
- Placed buildables now have stable session-local instance IDs.
- Save/load now persists gatherable depletion.
- Save/load now persists camera yaw and pitch.
- Placed buildable IDs round-trip through save/load.
- Runtime state identity for placed buildables and gatherable depletion is grouped in `SandboxWorldState`.

## Runtime State Ownership

`Game/Source/Runtime/SandboxWorldState` owns:

- placed buildable instances
- placed buildable instance ID allocation
- next placed ID continuation after load
- depleted gatherable ID collection
- depleted gatherable ID application during load

`SandboxGame` still coordinates gameplay updates. This pass intentionally avoids a broad rewrite.

## Gatherable Stable IDs

Mistwood Hollow assigns deterministic gatherable IDs in the handcrafted layout:

- `branch_pile_01`
- `branch_pile_02`
- `loose_stone_01`
- `fiber_plant_01`
- `cracked_rock_01`

Save/load stores depleted gatherables as:

```text
gatherable.depleted=branch_pile_01
```

Unknown gatherable IDs are ignored safely when loading.

## Placed Buildable Instance IDs

New placed buildables receive IDs like:

- `placed_0001`
- `placed_0002`

Save/load stores these IDs alongside the buildable type, position, and yaw:

```text
buildable.0=placed_0001,camp_marker,1,0,2,0
```

After loading, the next generated placed ID continues after the highest restored ID, so new placements do not collide.

## Camera Persistence

Save/load now persists:

- player position
- camera yaw
- camera pitch
- health
- stamina

The format uses:

```text
player.position=0,1.8,6
player.yaw=3.14159
player.pitch=-0.18
```

## Known Limitations

- Gatherable remaining counts are collapsed to depleted/not depleted for save/load.
- Placed buildables still do not have demolition, ownership, health, or repair state.
- Enemy save/load still stores only alive/dead and position.
- Hotbar assignments are not persisted yet.
- The current save format remains a simple prototype format.

## Intentionally Not Implemented

- new gameplay systems
- terrain or procedural generation
- data loading
- new biomes or enemies
- renderer changes
- real UI/HUD
- audio/VFX
- multiplayer
