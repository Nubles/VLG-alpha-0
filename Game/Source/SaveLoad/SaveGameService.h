#pragma once

#include "Game/Source/SaveLoad/SaveData.h"
#include "Game/Source/SaveLoad/SaveResult.h"
#include "Game/Source/SaveLoad/SaveSerializer.h"

#include <string>

namespace rw::game {

class SaveGameService {
public:
    static constexpr const char* kQuickSavePath = "Saves/quick_save.rwsave";

    static SaveResult SaveToFile(const SaveData& data, const std::string& path = kQuickSavePath);
    static LoadResult LoadFromFile(const std::string& path = kQuickSavePath);
};

} // namespace rw::game
