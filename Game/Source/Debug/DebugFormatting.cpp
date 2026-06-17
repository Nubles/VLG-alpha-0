#include "Game/Source/Debug/DebugFormatting.h"

#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Items/ItemDatabase.h"

#include <sstream>

namespace rw::game {

std::string CompactInventorySummary(const ItemDatabase& database, const Inventory& inventory)
{
    std::ostringstream summary;
    bool wroteAny = false;

    for (const ItemDefinition& definition : database.Definitions()) {
        const int quantity = inventory.TotalQuantity(definition.id);
        if (quantity <= 0) {
            continue;
        }

        if (wroteAny) {
            summary << ",";
        }

        summary << definition.id << ":" << quantity;
        wroteAny = true;
    }

    if (!wroteAny) {
        return "empty";
    }

    return summary.str();
}

} // namespace rw::game
