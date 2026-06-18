# Contextual Prompts And Guidance

Milestone 15 adds a lightweight game-side guidance layer for the current debug vertical slice. It uses the existing HUD/debug text overlay and does not add a quest system, tutorial framework, popup system, or new gameplay state.

## Purpose

The goal is to help a new tester understand what they can do now and what the next vertical-slice step is.

Examples:

- `PROMPT: E GATHER BRANCH PILE`
- `PROMPT: REQUIRES PRIMITIVE TOOL`
- `PROMPT: E STABILISE FRACTURE - REQS`
- `NEXT: CRAFT PRIMITIVE_TOOL`
- `NEXT: DEFEAT REALM WISP`
- `NEXT: CRAFT REALM_ANCHOR`

## Architecture

Guidance is game-side only:

- `ContextualPrompt` builds the current prompt from a small `ContextualPromptState`.
- `VerticalSliceGuide` builds the next-step line from a small `VerticalSliceGuideState`.
- `SandboxGame` derives both state structs from existing target, inventory, build, enemy, progression, and world state.
- `HudState` carries the prompt and next-step strings.
- `VerticalSliceHud` renders them through the existing rectangle-glyph debug text overlay.

The engine does not know about prompts, objectives, inventory, items, enemies, or the vertical-slice checklist.

## Prompt Line

The prompt line describes the immediate context:

- no target: explore Mistwood Hollow
- gatherable target: gather with `E`
- gatherable missing a required tool: show the required tool
- generic interactable: inspect with `E`
- Realm Fracture: stabilise or show stabilised state
- build mode: place, rotate, or cancel
- nearby Realm Wisp: attack with `C`

## Next-Step Line

The next-step line is a simple ordered checklist inferred from existing state:

1. enable mouse look with `M`
2. gather wood, stone, and fiber
3. craft `primitive_tool`
4. gather Cracked Rock
5. craft `camp_bundle` and `workbench_kit`
6. place a camp marker or workbench
7. defeat the Realm Wisp
8. stabilise the Realm Fracture
9. craft `realm_anchor`
10. save with `O` and load with `P`

This is not persistent tutorial tracking. It is derived from current runtime state.

## What This Is Not

This milestone does not add:

- quest log
- tutorial manager
- popups
- timers
- dialogue
- objective data loading
- new objectives
- new content
- inventory UI
- crafting menu
- mouse UI interaction

## Known Limitations

- The guidance is specific to the current Mistwood Hollow vertical slice.
- Text is intentionally short for the primitive rectangle-glyph font.
- Some guidance depends on debug prototype state and may be conservative.
- There is no persistent tutorial completion tracking.
- There is no authored narrative/tutorial flow.

## Future Path

A later scoped pass can move guidance into a proper tutorial/objective presentation layer, add better text layout, or introduce data-driven objective hints. That should happen only after the project deliberately chooses a UI/tutorial direction.
