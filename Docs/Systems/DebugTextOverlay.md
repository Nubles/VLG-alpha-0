# Debug Text Overlay

Milestone 13 adds a tiny dependency-free debug text overlay for the custom engine. It is meant to make the vertical slice readable in-game without relying only on the window title or console.

This is a debug text renderer, not a final UI system.

## Purpose

The overlay provides simple readable labels for:

- health and stamina
- current target
- objective/fracture status
- Realm Wisp name, health, and state
- build mode and selected buildable
- recent debug messages
- save/load result messages

## Architecture

Engine-side code is generic:

- `OverlayText` describes text, position, scale, colour, alpha, and optional max character count.
- `DebugTextRenderer::Sanitize` normalises text for the built-in glyph set.
- `DebugTextRenderer::BuildTextRects` turns text into `OverlayRect` draw commands.
- `Renderer::RenderOverlay` draws those rectangles with the existing fixed-function OpenGL overlay path.

Game-side HUD code composes the text:

- `VerticalSliceHud::BuildTextLines` creates the current readable HUD strings from `HudState`.
- `VerticalSliceHud::BuildTextOverlay` converts those strings into rectangle glyphs.
- `SandboxGame` still builds `HudState` from existing vertical-slice state.

The engine does not know about health, stamina, objectives, items, enemies, or any gameplay concepts.

## Supported Characters

The built-in 5x7 monospaced glyph table supports:

- uppercase `A-Z`
- digits `0-9`
- space
- colon `:`
- slash `/`
- dash `-`
- underscore `_`
- period `.`
- comma `,`
- exclamation `!`
- question mark `?`
- parentheses `(` and `)`

Lowercase letters are converted to uppercase before rendering. Unsupported characters become `?`. Newlines, carriage returns, and tabs are converted to spaces.

## What HUD Text Is Shown

The in-game HUD text currently shows:

- `HP current/max`
- `ST current/max`
- `TARGET: name`
- `OBJECTIVE: status`
- enemy summary when the Realm Wisp is relevant
- build mode summary when build mode is active
- latest debug messages
- save/load result when present

The older window-title summary and console/debug logs remain active.

## Why This Is Not A Full UI System

This milestone does not add:

- real font loading
- asset loading
- layout containers
- inventory grids
- mouse interaction
- drag/drop
- menus
- ImGui, FreeType, DirectWrite, or any third-party UI library

The renderer is intentionally primitive and uses rectangles because the project does not yet have an asset or UI pipeline.

## Limitations

- The font is small and blocky.
- Text is uppercase-only after sanitization.
- There is no wrapping beyond max character truncation.
- There is no kerning, alignment, measuring API, or clipping.
- The HUD still uses fixed screen positions.
- Long debug messages are truncated.

## Future Path

A later scoped UI pass can replace or supplement this with:

- real font rendering
- a UI layout model
- text measurement and wrapping
- HUD widgets
- inventory UI
- mouse/focus handling
- menu screens

Those should be deliberate milestones, not accidental growth from this debug overlay.
