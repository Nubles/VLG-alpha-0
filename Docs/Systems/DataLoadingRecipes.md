# Recipe Definition Data Loading

Stabilisation Pass 4 adds a minimal dependency-free data-loading path for recipe definitions.

## Data File Path

Runtime recipe data lives at:

`Game/Data/Recipes/recipes.txt`

The CMake build already copies `Game/Data` beside the `RealmboundWilds` executable after build, so the recipe file follows the same runtime path as item data.

## File Format

The file is pipe-delimited and line-based:

```text
# id|display_name|output_item_id|output_quantity|required_station_id|required_unlock_flag|ingredients
primitive_tool|Primitive Tool|primitive_tool|1|||wood:1,stone:1,fiber:1
```

Rules:

- empty lines are ignored
- lines starting with `#` are ignored
- every recipe row must have exactly 7 fields
- fields are ordered as `id`, `display_name`, `output_item_id`, `output_quantity`, `required_station_id`, `required_unlock_flag`, `ingredients`
- `required_station_id` may be empty
- `required_unlock_flag` may be empty

## Ingredient Syntax

Ingredients are comma-separated `item_id:quantity` pairs:

```text
wood:5,fiber:3
```

Ingredient quantities must be positive integers.

## Optional Station And Unlock Fields

The parser preserves `required_station_id` and `required_unlock_flag` as stable strings.

The current vertical slice uses `required_unlock_flag` for `realm_anchor`:

`recipe_realm_anchor_unlocked`

## Fallback Behavior

`RecipeDatabase::CreateFromFileOrFallback(path)` tries to load recipe definitions from the data file first.

If the file is missing or invalid, the game logs a warning and falls back to `RecipeDatabase::CreateStarterRecipes()`. The fallback intentionally mirrors the current vertical-slice recipes so gameplay can still run.

## Validation Behavior

The parser rejects invalid data with a clear message:

- missing file
- malformed field count
- empty required fields
- invalid output quantity
- empty ingredient list
- malformed ingredient pair
- invalid ingredient quantity
- no recipe definitions loaded

The parser returns `RecipeDatabaseLoadResult` instead of crashing.

## Current Limitations

- This is not a generic data system.
- Whitespace trimming and escaping are not implemented.
- Duplicate recipe ID handling is not special-cased yet.
- Output and ingredient item IDs are stable strings and are not cross-validated against the item database during parsing.
- Data loading covers recipe definitions only.
- Buildables, biome layouts, drops, and objectives remain code-defined.

## Why Buildables, Biomes, Drops, And Objectives Stay In Code

This pass moves one definition type at a time. Recipes are the next safest step after items because crafting already consumes stable item IDs and progression already checks a stable unlock flag.

Buildables should be the next data-loading candidate after this pass is reviewed and CI remains green.

