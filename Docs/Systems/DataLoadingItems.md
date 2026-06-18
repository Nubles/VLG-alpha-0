# Item Definition Data Loading

Stabilisation Pass 3 adds a minimal dependency-free data-loading path for item definitions.

## Data File Path

Runtime item data lives at:

`Game/Data/Items/items.txt`

The CMake build copies `Game/Data` beside the `RealmboundWilds` executable after build, so the same relative path is available when running from either the repository root or the build output directory.

## File Format

The file is pipe-delimited and line-based:

```text
# id|display_name|description|category|max_stack|tier
wood|Wood|A sturdy piece of weathered timber.|Resource|50|0
```

Rules:

- empty lines are ignored
- lines starting with `#` are ignored
- every item row must have exactly 6 fields
- fields are ordered as `id`, `display_name`, `description`, `category`, `max_stack`, `tier`

## Supported Categories

- `Resource`
- `Food`
- `Tool`

These map directly to the existing `ItemCategory` enum.

## Fallback Behavior

`ItemDatabase::CreateFromFileOrFallback(path)` tries to load item definitions from the data file first.

If the file is missing or invalid, the game logs a warning and falls back to `ItemDatabase::CreateStarterDatabase()`. The hardcoded fallback intentionally mirrors the current vertical-slice definitions so gameplay can still run.

## Validation Behavior

The parser rejects invalid data with a clear message:

- missing file
- malformed field count
- empty required field
- unknown category
- invalid numeric field
- `max_stack` less than or equal to zero
- no item definitions loaded

The parser returns `ItemDatabaseLoadResult` instead of crashing.

## Current Limitations

- This is not a generic data system.
- Whitespace trimming and escaping are not implemented.
- Duplicate item ID handling is not special-cased yet.
- Data loading covers item definitions only.
- Recipes, buildables, biome layouts, drops, and objectives remain code-defined.

## Why Recipes, Buildables, And Biomes Stay In Code

This pass keeps risk low by moving one definition type at a time. Items are the safest first step because inventory, gathering, crafting, building costs, objectives, enemy drops, and save/load already refer to stable item IDs.

Recipes should be the next data-loading candidate after this pass is reviewed and CI remains green.

