#include "Game/Source/SaveLoad/SaveGameService.h"

#include "Game/Source/SaveLoad/SaveSerializer.h"

#include <filesystem>
#include <fstream>
#include <sstream>

namespace rw::game {

SaveResult SaveGameService::SaveToFile(const SaveData& data, const std::string& path)
{
    const std::filesystem::path savePath(path);
    const std::filesystem::path directory = savePath.parent_path();
    if (!directory.empty()) {
        std::filesystem::create_directories(directory);
    }

    std::ofstream file(savePath, std::ios::trunc);
    if (!file.is_open()) {
        return { false, "Save failed: cannot write " + path };
    }

    file << SaveSerializer::Serialize(data);
    return { true, "Saved game to " + path };
}

LoadResult SaveGameService::LoadFromFile(const std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open()) {
        return { false, "Load failed: missing " + path, {} };
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return SaveSerializer::Deserialize(buffer.str());
}

} // namespace rw::game
