#include "Game/Source/SaveLoad/SaveSerializer.h"

#include <sstream>
#include <string>
#include <vector>

namespace rw::game {

namespace {

std::string BoolText(bool value)
{
    return value ? "true" : "false";
}

bool ParseBool(const std::string& text, bool& outValue)
{
    if (text == "true") {
        outValue = true;
        return true;
    }
    if (text == "false") {
        outValue = false;
        return true;
    }
    return false;
}

std::string Vec3Text(const rw::math::Vec3& value)
{
    std::ostringstream text;
    text << value.x << "," << value.y << "," << value.z;
    return text.str();
}

std::vector<std::string> Split(const std::string& value, char delimiter)
{
    std::vector<std::string> parts;
    std::stringstream stream(value);
    std::string part;
    while (std::getline(stream, part, delimiter)) {
        parts.push_back(part);
    }
    return parts;
}

bool ParseVec3(const std::string& text, rw::math::Vec3& outValue)
{
    const std::vector<std::string> parts = Split(text, ',');
    if (parts.size() != 3) {
        return false;
    }

    try {
        outValue = {
            std::stof(parts[0]),
            std::stof(parts[1]),
            std::stof(parts[2]),
        };
    } catch (...) {
        return false;
    }
    return true;
}

bool StartsWith(const std::string& text, const std::string& prefix)
{
    return text.rfind(prefix, 0) == 0;
}

} // namespace

std::string SaveSerializer::Serialize(const SaveData& data)
{
    std::ostringstream text;
    text << "version=" << data.version << "\n";
    text << "biome=" << data.biomeId << "\n";
    text << "player.position=" << Vec3Text(data.playerPosition) << "\n";
    text << "player.health=" << data.playerHealth << "\n";
    text << "player.stamina=" << data.playerStamina << "\n";

    for (size_t index = 0; index < data.inventorySlots.size(); ++index) {
        const ItemStack& slot = data.inventorySlots[index];
        if (!slot.IsEmpty()) {
            text << "inventory.slot." << index << "=" << slot.itemId << "," << slot.quantity << "\n";
        }
    }

    for (const std::string& flag : data.progressionFlags) {
        text << "progression.flag=" << flag << "\n";
    }

    for (size_t index = 0; index < data.placedBuildables.size(); ++index) {
        const SavedBuildable& buildable = data.placedBuildables[index];
        text << "buildable." << index << "=" << buildable.buildableId
             << "," << buildable.position.x
             << "," << buildable.position.y
             << "," << buildable.position.z
             << "," << buildable.yawRadians << "\n";
    }

    text << "objective.completed=" << BoolText(data.mistwoodObjectiveCompleted) << "\n";
    text << "fracture.stabilized=" << BoolText(data.mistwoodFractureStabilized) << "\n";
    text << "enemy.realm_wisp.alive=" << BoolText(data.realmWispAlive) << "\n";
    text << "enemy.realm_wisp.position=" << Vec3Text(data.realmWispPosition) << "\n";
    return text.str();
}

LoadResult SaveSerializer::Deserialize(const std::string& text)
{
    SaveData data;
    bool sawVersion = false;
    bool sawBiome = false;

    std::stringstream stream(text);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) {
            continue;
        }

        const size_t equals = line.find('=');
        if (equals == std::string::npos) {
            return { false, "Invalid save line: " + line, {} };
        }

        const std::string key = line.substr(0, equals);
        const std::string value = line.substr(equals + 1);

        try {
            if (key == "version") {
                data.version = std::stoi(value);
                sawVersion = true;
                if (data.version != 1) {
                    return { false, "Unsupported save version", {} };
                }
            } else if (key == "biome") {
                data.biomeId = value;
                sawBiome = true;
            } else if (key == "player.position") {
                if (!ParseVec3(value, data.playerPosition)) {
                    return { false, "Invalid player position", {} };
                }
            } else if (key == "player.health") {
                data.playerHealth = std::stof(value);
            } else if (key == "player.stamina") {
                data.playerStamina = std::stof(value);
            } else if (StartsWith(key, "inventory.slot.")) {
                const int index = std::stoi(key.substr(15));
                const std::vector<std::string> parts = Split(value, ',');
                if (index < 0 || parts.size() != 2) {
                    return { false, "Invalid inventory slot", {} };
                }
                if (data.inventorySlots.size() <= static_cast<size_t>(index)) {
                    data.inventorySlots.resize(static_cast<size_t>(index) + 1);
                }
                data.inventorySlots[static_cast<size_t>(index)] = { parts[0], std::stoi(parts[1]) };
            } else if (key == "progression.flag") {
                data.progressionFlags.push_back(value);
            } else if (StartsWith(key, "buildable.")) {
                const std::vector<std::string> parts = Split(value, ',');
                if (parts.size() != 5) {
                    return { false, "Invalid buildable", {} };
                }
                data.placedBuildables.push_back({
                    parts[0],
                    { std::stof(parts[1]), std::stof(parts[2]), std::stof(parts[3]) },
                    std::stof(parts[4]),
                });
            } else if (key == "objective.completed") {
                if (!ParseBool(value, data.mistwoodObjectiveCompleted)) {
                    return { false, "Invalid objective state", {} };
                }
            } else if (key == "fracture.stabilized") {
                if (!ParseBool(value, data.mistwoodFractureStabilized)) {
                    return { false, "Invalid fracture state", {} };
                }
            } else if (key == "enemy.realm_wisp.alive") {
                if (!ParseBool(value, data.realmWispAlive)) {
                    return { false, "Invalid wisp state", {} };
                }
            } else if (key == "enemy.realm_wisp.position") {
                if (!ParseVec3(value, data.realmWispPosition)) {
                    return { false, "Invalid wisp position", {} };
                }
            }
        } catch (...) {
            return { false, "Invalid save value: " + line, {} };
        }
    }

    if (!sawVersion || !sawBiome) {
        return { false, "Missing required save header", {} };
    }

    return { true, "Loaded save data", data };
}

} // namespace rw::game
