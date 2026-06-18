# Buildable Definition Data Loading

Stabilisation Pass 5 adds a minimal dependency-free data-loading path for buildable definitions.

## Data File Path

Runtime buildable data lives at:

`Game/Data/Buildables/buildables.txt`

The CMake build copies `Game/Data` beside the `RealmboundWilds` executable after build, so buildable data follows the same runtime path as item and recipe data.

## File Format

The file is pipe-delimited and line-based:

```text
# id|display_name|category|primitive|costs|preview_scale|placed_scale|placement_distance|placement_radius
camp_marker|Camp Marker|Camp|Cube|camp_bundle:1|0.8,1,0.8|0.8,1,0.8|3.0|0.75
```

Fields are ordered as:

- `id`
- `display_name`
- `category`
- `primitive`
- `costs`
- `preview_scale`
- `placed_scale`
- `placement_distance`
- `placement_radius`

Empty lines and lines starting with `#` are ignored.

## Cost Syntax

Costs are comma-separated `item_id:quantity` pairs:

```text
wood:3,fiber:1
```

Cost quantities must be positive integers.

## Scale Syntax

`preview_scale` and `placed_scale` use `x,y,z` triples:

```text
2,1.8,0.25
```

All scale values must be positive.

## Category And Primitive Support

Categories are stored as stable strings and are not restricted yet.

The only supported primitive name is currently:

- `Cube`

That mirrors the current debug-rendered buildables.

## Fallback Behavior

`BuildableDatabase::CreateFromFileOrFallback(path)` tries to load buildable definitions from the data file first.

If the file is missing or invalid, the game logs a warning and falls back to `BuildableDatabase::CreateStarterBuildables()`.

## Validation Behavior

The parser rejects invalid data with a clear message:

- missing file
- malformed field count
- empty required fields
- unknown primitive
- malformed cost pair
- invalid cost quantity
- malformed scale triple
- non-positive scale values
- invalid placement distance
- invalid placement radius
- no buildable definitions loaded

The parser returns `BuildableDatabaseLoadResult` instead of crashing.

## Current Limitations

- This is not a generic data system.
- Whitespace trimming and escaping are not implemented.
- Duplicate buildable ID handling is not special-cased yet.
- Cost item IDs are stable strings and are not cross-validated against the item database during parsing.
- Data loading covers buildable definitions only.
- Biome layouts, drops, and objectives remain code-defined.

## Why Biome Layouts, Drops, And Objectives Stay In Code

This pass keeps risk low by moving one definition type at a time. Buildables are a natural step after items and recipes because placement already uses stable buildable IDs, stable item costs, and save/load already stores placed buildable instance IDs separately.

Biome layouts, drop tables, and objectives should only move to data in their own scoped passes after this work is reviewed and CI remains green.

