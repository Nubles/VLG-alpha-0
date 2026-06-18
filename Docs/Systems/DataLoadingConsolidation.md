# Data Loading Consolidation

Stabilisation Pass 6 consolidates the tiny text-parsing helpers used by item, recipe, and buildable data loading.

## Shared Helpers

The shared helpers live in:

- `Game/Source/Data/DataTextParser.h`
- `Game/Source/Data/DataTextParser.cpp`

Available helpers:

- `Trim`
- `Split`
- `IsCommentOrBlankLine`
- `ReadFileText`
- `MakeLineError`
- `ParsePositiveInt`
- `ParsePositiveFloat`
- `ParseVec3Triple`

## Systems Using The Helpers

The following loaders now use `DataTextParser`:

- `ItemDatabase`
- `RecipeDatabase`
- `BuildableDatabase`

Each database still owns its domain-specific validation and fallback behavior.

## File Formats

The existing file formats are unchanged:

- `Game/Data/Items/items.txt`
- `Game/Data/Recipes/recipes.txt`
- `Game/Data/Buildables/buildables.txt`

All remain pipe-delimited, line-based files.

## Fallback Behavior

Fallback behavior is unchanged:

- item loading falls back to `ItemDatabase::CreateStarterDatabase`
- recipe loading falls back to `RecipeDatabase::CreateStarterRecipes`
- buildable loading falls back to `BuildableDatabase::CreateStarterBuildables`

Failures still log a warning and continue with hardcoded definitions.

## Validation Behavior

The shared helper handles common validation primitives:

- comments and blank lines
- trimmed fields
- positive integers
- positive floats
- positive `Vec3` triples
- consistent line-error formatting

Domain loaders still validate their own field counts, required fields, categories, primitives, ingredient syntax, cost syntax, and required unlock fields.

## Current Limitations

- No reflection.
- No schema system.
- No generic table database.
- No JSON, YAML, or TOML.
- No asset manager.
- No hot reload.
- No editor tooling.
- No escaping for delimiters.
- Duplicate IDs are still documented as a future validation improvement.

## Intentionally Not Implemented

This pass does not move biome layouts, drop tables, or objectives to data. It only reduces repeated parsing helper code for the data loaders that already exist.
