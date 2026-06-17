#pragma once

#include <string>

namespace rw::game {

class Inventory;
class ItemDatabase;

std::string CompactInventorySummary(const ItemDatabase& database, const Inventory& inventory);

} // namespace rw::game
