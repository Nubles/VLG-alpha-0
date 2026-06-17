#pragma once

#include "Game/Source/SaveLoad/SaveData.h"
#include "Game/Source/SaveLoad/SaveResult.h"

#include <string>

namespace rw::game {

struct LoadResult {
    bool success = false;
    std::string message;
    SaveData data;
};

class SaveSerializer {
public:
    static std::string Serialize(const SaveData& data);
    static LoadResult Deserialize(const std::string& text);
};

} // namespace rw::game
