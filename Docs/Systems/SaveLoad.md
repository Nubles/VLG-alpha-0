# Save Load

Milestone 10 adds a small human-readable save/load system for the current vertical slice. It is deterministic, line-based, dependency-free, and intentionally limited to the state the prototype currently owns.

## Architecture

Save/load code lives in `Game/Source/SaveLoad`.

- `SaveData` is the plain data snapshot.
- `SaveSerializer` converts `SaveData` to and from a line-based text format.
- `SaveGameService` writes and reads the quick-save file.
- `SandboxGame` captures runtime state into `SaveData` and applies loaded state back into the scene.

No third-party serializer, binary format, encryption, cloud save, or multi-slot system is used.

## Save Path

Quick save path:

`Saves/quick_save.rwsave`

The save service creates the `Saves` directory if it does not exist.

## Save Format

The format is simple `key=value` text.

Example:

```text
version=1
biome=mistwood_hollow
player.position=0,1.8,6
player.yaw=3.14159
player.pitch=-0.18
player.health=100
player.stamina=85
inventory.slot.0=wood,12
progression.flag=mistwood_fracture_discovered
buildable.0=placed_0001,camp_marker,1,0,2,0
gatherable.depleted=mistwood_branch_01
objective.completed=true
fracture.stabilized=true
enemy.realm_wisp.alive=false
enemy.realm_wisp.position=1.2,0.7,-5
```

Unknown optional keys are ignored for now. Missing required header fields or unsupported versions fail gracefully.

## Persisted Fields

Persisted now:

- save version
- current biome ID
- player position
- player camera yaw and pitch
- player health
- player stamina
- inventory slots
- progression flags
- placed buildables with instance IDs
- depleted gatherable IDs
- Mistwood objective completion state
- Realm Fracture stabilized state
- Realm Wisp alive/dead state
- Realm Wisp position

Gatherable depletion is persisted through stable gatherable IDs from the handcrafted biome layout.

## Debug Controls

- O: save to `Saves/quick_save.rwsave`
- P: load from `Saves/quick_save.rwsave`

Existing controls remain unchanged.

## Known Limitations

- One quick-save file only.
- No autosave.
- No save slot UI.
- No binary compression or encryption.
- No procedural world, terrain chunk, or future-system persistence.
- Loading assumes the current handcrafted biome layout exists.
- Save/load is still tied to the current handcrafted biome layout.

## Not Implemented Yet

- multiple save slots
- cloud saves
- settings persistence
- autosave timers
- asset loading
- multiplayer persistence
- full advanced serialization framework
- final UI
