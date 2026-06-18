# UI HUD Foundation

Milestone 12 adds the first dependency-free HUD path for the custom engine vertical slice. It is intentionally small: the HUD uses simple fixed-function OpenGL rectangle overlays and keeps detailed text in the existing window title and console/debug message paths.

Milestone 13 builds on this by adding primitive dependency-free debug text rendered from rectangle glyphs. The HUD now shows readable in-game labels while the title and console paths remain active.

## Architecture

Engine-side rendering stays generic:

- `rw::renderer::OverlayRect` describes a 2D rectangle in screen pixels.
- `Renderer::RenderOverlay` draws overlay rectangles after the 3D scene.
- `Renderer::Present` swaps the back buffer after both 3D scene rendering and HUD rendering are complete.

Game-side HUD composition stays in `Game/Source/UI`:

- `HudState` is a plain snapshot of vertical-slice HUD data.
- `HudFormatter` owns pure formatting and clamped ratio helpers.
- `VerticalSliceHud` converts `HudState` into generic renderer overlay rectangles.
- `DebugTextRenderer` converts small built-in glyph text into overlay rectangles.
- `SandboxGame` builds `HudState` from existing player, inventory, objective, enemy, build, message, and save/load state.

## What Is Rendered

The visible HUD currently renders:

- health bar
- stamina bar
- health and stamina text labels
- target panel highlight when an interactable or gatherable is targeted
- target name text
- objective/fracture status panel placeholder
- objective/fracture status text
- enemy health bar when the Realm Wisp has health
- enemy text summary
- build mode indicator placeholder when build mode is active
- build mode text
- message/save panel placeholder when recent messages exist
- recent debug message text

## What Remains In Debug Title And Console

Detailed strings still remain in the window title and console/debug log:

- exact health and stamina numbers
- target name
- compact inventory summary
- enemy state
- objective status
- selected buildable name
- recent debug message summary
- save/load result text

The title path is preserved so testers still have readable information while the HUD gains visual indicators.

## Why Real Text Rendering Is Deferred

The project now has primitive rectangle-glyph debug text, but it does not yet have an asset pipeline, font loading, or a UI layout system. Adding a font library, ImGui, or a third-party UI dependency would be larger than this milestone. Real text rendering should be introduced only when the project deliberately accepts the asset and layout scope.

## Controls

No gameplay controls changed in this milestone.

Existing relevant controls:

- `WASD`: move
- `Arrow keys`: look
- `Shift`: sprint
- `E`: interact/gather/objective
- `C`: attack
- `B`: build mode
- `1`, `2`, `3`: select buildable
- `Q`, `R`: rotate build preview
- `Enter`: place buildable
- `O`: save
- `P`: load
- `H`: help/debug message
- `Escape`: exit

## Known Limitations

- no real text rendering
- no inventory grid UI
- no mouse interaction
- no drag/drop
- no UI focus model
- no layout anchors beyond simple fixed rectangle placement
- no external fonts or assets
- debug title and console still carry most detailed text

## Manual Test Checklist

1. App opens.
2. Mistwood Hollow loads.
3. HUD overlay is visible.
4. Health indicator updates when Realm Wisp damages the player.
5. Stamina indicator drains while sprinting and regenerates afterward.
6. Target/objective/message indicators are visible or still clearly available through debug title/console.
7. Build mode indicator appears when build mode is active.
8. Existing `H` help still works.
9. Existing gather/craft/build/fight/objective/save/load flow still works.
10. Existing vertical-slice happy path still works.
11. GitHub Actions is green.
12. Escape exits cleanly.
