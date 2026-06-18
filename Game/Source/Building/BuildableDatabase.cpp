#include "Game/Source/Building/BuildableDatabase.h"

#include "Engine/Core/Logger.h"
#include "Game/Source/Data/DataTextParser.h"

#include <sstream>
#include <string>
#include <utility>
#include <vector>

namespace rw::game {

namespace {

bool ParsePrimitive(const std::string& text, rw::scene::DebugPrimitive& outPrimitive)
{
    if (text == "Cube") {
        outPrimitive = rw::scene::DebugPrimitive::Cube;
        return true;
    }
    return false;
}

bool ParseCosts(
    const std::string& text,
    int lineNumber,
    std::vector<BuildCost>& outCosts,
    std::string& outError)
{
    const std::vector<std::string> costTexts = data::Split(text, ',');
    if (costTexts.empty()) {
        outError = data::MakeLineError("Buildable", lineNumber, "cost list must not be empty");
        return false;
    }

    for (const std::string& costText : costTexts) {
        const std::vector<std::string> parts = data::Split(costText, ':');
        if (parts.size() != 2 || parts[0].empty() || parts[1].empty()) {
            outError = data::MakeLineError("Buildable", lineNumber, "Invalid cost '" + costText + "'");
            return false;
        }

        int quantity = 0;
        if (!data::ParsePositiveInt(parts[1], quantity)) {
            outError = data::MakeLineError("Buildable", lineNumber, "cost quantity must be greater than 0");
            return false;
        }

        outCosts.push_back({ parts[0], quantity });
    }

    return true;
}

bool ParsePositiveScale(
    const std::string& text,
    const std::string& fieldName,
    int lineNumber,
    rw::math::Vec3& outScale,
    std::string& outError)
{
    if (!data::ParseVec3Triple(text, outScale)) {
        outError = "Buildable data line " + std::to_string(lineNumber) + ": Invalid " + fieldName;
        return false;
    }
    return true;
}

bool HasRequiredBuildableFields(const std::vector<std::string>& fields)
{
    for (const std::string& field : fields) {
        if (field.empty()) {
            return false;
        }
    }
    return true;
}

} // namespace

BuildableDatabase BuildableDatabase::CreateStarterBuildables()
{
    BuildableDatabase database;

    database.AddBuildable({
        "camp_marker",
        "Camp Marker",
        { { "camp_bundle", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 0.8F, 1.0F, 0.8F },
        { 0.8F, 1.0F, 0.8F },
        3.0F,
        0.75F,
        "Camp",
    });

    database.AddBuildable({
        "workbench_stub",
        "Workbench Stub",
        { { "workbench_kit", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 1.8F, 0.8F, 0.9F },
        { 1.8F, 0.8F, 0.9F },
        3.0F,
        1.15F,
        "Stations",
    });

    database.AddBuildable({
        "simple_wall",
        "Simple Wall",
        { { "wood", 3 }, { "fiber", 1 } },
        rw::scene::DebugPrimitive::Cube,
        { 2.0F, 1.8F, 0.25F },
        { 2.0F, 1.8F, 0.25F },
        3.0F,
        1.05F,
        "Walls",
    });

    return database;
}

BuildableDatabaseLoadResult BuildableDatabase::LoadFromText(const std::string& text)
{
    BuildableDatabase database;
    std::stringstream stream(text);
    std::string line;
    int lineNumber = 0;

    while (std::getline(stream, line)) {
        ++lineNumber;
        if (data::IsCommentOrBlankLine(line)) {
            continue;
        }

        const std::vector<std::string> fields = data::Split(line, '|');
        if (fields.size() != 9) {
            return {
                false,
                data::MakeLineError("Buildable", lineNumber, "Expected 9 fields"),
                {},
            };
        }

        if (!HasRequiredBuildableFields(fields)) {
            return {
                false,
                data::MakeLineError("Buildable", lineNumber, "Required field is empty"),
                {},
            };
        }

        rw::scene::DebugPrimitive primitive = rw::scene::DebugPrimitive::Cube;
        if (!ParsePrimitive(fields[3], primitive)) {
            return {
                false,
                data::MakeLineError("Buildable", lineNumber, "Unknown primitive '" + fields[3] + "'"),
                {},
            };
        }

        std::vector<BuildCost> costs;
        std::string costError;
        if (!ParseCosts(fields[4], lineNumber, costs, costError)) {
            return { false, costError, {} };
        }

        rw::math::Vec3 previewScale;
        rw::math::Vec3 placedScale;
        std::string scaleError;
        if (!ParsePositiveScale(fields[5], "preview_scale", lineNumber, previewScale, scaleError)) {
            return { false, scaleError, {} };
        }
        if (!ParsePositiveScale(fields[6], "placed_scale", lineNumber, placedScale, scaleError)) {
            return { false, scaleError, {} };
        }

        float placementDistance = 0.0F;
        if (!data::ParsePositiveFloat(fields[7], placementDistance)) {
            return {
                false,
                data::MakeLineError("Buildable", lineNumber, "placement_distance must be greater than 0"),
                {},
            };
        }

        float placementRadius = 0.0F;
        if (!data::ParsePositiveFloat(fields[8], placementRadius)) {
            return {
                false,
                data::MakeLineError("Buildable", lineNumber, "placement_radius must be greater than 0"),
                {},
            };
        }

        database.AddBuildable({
            fields[0],
            fields[1],
            costs,
            primitive,
            previewScale,
            placedScale,
            placementDistance,
            placementRadius,
            fields[2],
        });
    }

    if (database.Buildables().empty()) {
        return { false, "Buildable data did not contain any buildable definitions", {} };
    }

    return { true, "Loaded buildable definitions", database };
}

BuildableDatabaseLoadResult BuildableDatabase::LoadFromFile(const std::string& path)
{
    bool readSuccess = false;
    const std::string text = data::ReadFileText(path, readSuccess);
    if (!readSuccess) {
        return { false, "Could not open buildable data file: " + path, {} };
    }
    return LoadFromText(text);
}

BuildableDatabase BuildableDatabase::CreateFromFileOrFallback(const std::string& path)
{
    const BuildableDatabaseLoadResult loadResult = LoadFromFile(path);
    if (loadResult.success) {
        return loadResult.database;
    }

    rw::core::Logger::Warning(loadResult.message + "; using hardcoded starter buildable definitions.");
    return CreateStarterBuildables();
}

void BuildableDatabase::AddBuildable(BuildableDefinition buildable)
{
    m_buildables.push_back(std::move(buildable));
}

const BuildableDefinition* BuildableDatabase::FindById(const std::string& buildableId) const
{
    for (const BuildableDefinition& buildable : m_buildables) {
        if (buildable.id == buildableId) {
            return &buildable;
        }
    }

    return nullptr;
}

const std::vector<BuildableDefinition>& BuildableDatabase::Buildables() const
{
    return m_buildables;
}

} // namespace rw::game
