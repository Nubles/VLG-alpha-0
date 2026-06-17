# Vertical Slice Polish

Milestone 11 tightens the current Realmbound Wilds vertical slice without adding new major gameplay systems. The focus is controls discoverability, compact debug state, recent-message feedback, and a clear manual review flow.

## Controls

- WASD: move
- Arrow keys: look
- Shift: sprint
- E: interact, gather, or progress the Realm Fracture objective
- C: melee attack
- B: toggle build mode
- 1: select Camp Marker
- 2: select Workbench Stub
- 3: select Simple Wall
- Q/R: rotate build preview
- Enter: place selected buildable
- Backspace: cancel build mode
- F1: grant wood
- F2: grant stone
- F3: grant fiber
- F4: grant raw_food
- F5: grant primitive_tool
- F6: craft primitive_tool
- F7: craft camp_bundle
- F8: craft simple_meal
- F9: craft workbench_kit
- F10: reset Realm Wisp
- F11: craft realm_anchor
- F12: reset progression/objective state
- O: save
- P: load
- H: print/show debug help
- Escape: exit

## Debug Features

- H pushes a compact help string to the message log and console.
- The title shows biome, health, stamina, target, compact inventory, enemy state/health, objective status, build selection when active, and recent messages.
- Debug messages keep a bounded recent history instead of only one message per subsystem.
- Compact inventory summaries use item IDs and quantities, such as `wood:10,fiber:3`.

## Vertical Slice Happy Path

1. Spawn in Mistwood Hollow.
2. Gather wood, stone, fiber, and raw_food.
3. Craft primitive_tool.
4. Gather Cracked Rock with primitive_tool.
5. Craft camp_bundle and workbench_kit.
6. Place camp_marker or workbench_stub.
7. Fight Realm Wisp.
8. Stabilise Realm Fracture.
9. Craft realm_anchor.
10. Save.
11. Change state.
12. Load.
13. Confirm restored position, inventory, progression, placed buildables, fracture state, and Realm Wisp state.

## Current Debug State

The window title is intentionally compact and debug-oriented. It is not final UI. Console logging still mirrors key messages for review.

## Known Limitations

- No final HUD or menu UI.
- No mouse-look.
- No terrain mesh, terrain chunks, or procedural generation.
- No textures, models, or asset loading.
- No audio or polished VFX.
- No boss system, new biome, multiplayer, or complex quest system.
- Save/load remains one quick-save slot.

## Manual Review Checklist

1. App opens.
2. H shows/prints help.
3. Mistwood Hollow loads.
4. Player can gather starter resources.
5. Player can craft primitive_tool.
6. Player can gather Cracked Rock after getting primitive_tool.
7. Player can craft camp_bundle and workbench_kit.
8. Player can place camp_marker/workbench_stub.
9. Realm Wisp can attack player.
10. Player can kill Realm Wisp.
11. Loot enters inventory.
12. Realm Fracture reports missing requirements.
13. Player can stabilise Realm Fracture.
14. realm_anchor recipe unlocks.
15. Player can craft realm_anchor.
16. Save works.
17. Load works.
18. Debug messages are clear.
19. GitHub Actions is green.
20. Escape exits cleanly.
