# Mouse Look Input

Milestone 14 adds a small dependency-free mouse-look foundation while preserving arrow-key look as the fallback.

## Architecture

Engine-side input remains generic:

- `InputState` stores mouse position, per-frame mouse delta, and whether mouse capture is enabled.
- `Window::PollEvents` fills keyboard and mouse state from Win32.
- Win32-specific cursor hiding, clipping, and recentering stay in `Engine/Platform/Window.cpp`.

Game-side interpretation stays in gameplay code:

- `SandboxGame` toggles mouse look with `M` and stores the current enabled state.
- `PlayerController` applies mouse delta to camera yaw/pitch only when mouse capture is enabled.
- Arrow-key look still works as a keyboard fallback.

## Controls

- `M`: toggle mouse look on/off
- `Arrow keys`: keyboard look fallback
- `WASD`: movement
- `Shift`: sprint
- `Escape`: exit

All existing vertical-slice controls remain unchanged.

## Platform Behavior

Mouse look is off by default. When enabled on Windows:

- the cursor is hidden
- the cursor is clipped to the game window
- the cursor is recentered each frame
- the frame's mouse delta is passed through `InputState`

When disabled, the cursor is shown again and clipping is released.

## What Is Implemented

- mouse position tracking
- per-frame mouse delta
- optional relative-look mode
- `M` toggle
- Win32 cursor hide/confine/recenter behavior
- mouse yaw/pitch camera control
- arrow-key look fallback
- pitch clamp
- HUD/title/help text indicating mouse-look state/control

## Deferred

- settings menu
- key rebinding UI
- full input action mapping system
- controller support
- mouse interaction with UI
- drag/drop
- final camera/input tuning

## Known Limitations

- Mouse sensitivity is currently a hardcoded debug constant.
- Mouse capture state is toggled from game code and applied by the platform layer on the following poll cycle.
- Cursor capture is currently implemented only for the Win32 backend.
- There is no settings screen or user-facing sensitivity option yet.
