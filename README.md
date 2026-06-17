# Realmbound Wilds

Realmbound Wilds is a survival crafting game built on a small project-specific custom C++ engine.

The current repository state is foundation-only: docs, permanent agent instructions, and the proposed CMake/folder structure. Gameplay code has not started yet.

## Direction

- Build a focused custom engine for this game, not a general-purpose engine.
- Use C++20 and CMake.
- Keep `Engine` and `Game` separated.
- Use primitive rendering and placeholder assets before final art.
- Build toward a small vertical slice before expanding content.

## Next Milestone

Milestone 0: Custom Engine Bootstrap.

The next implementation should add:

- Engine/Game CMake targets.
- Window creation.
- Main loop.
- Logging.
- Time and delta-time.
- Input polling.
- Blank screen.
- Debug overlay placeholder.
- Build/run documentation.

