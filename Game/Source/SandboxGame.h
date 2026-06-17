#pragma once

#include "Engine/Core/GameLayer.h"
#include "Engine/Scene/Scene.h"
#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Interactable.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/PlayerController.h"
#include "Game/Source/Resources/GatherableNode.h"

#include <string>
#include <vector>

namespace rw::game {

class SandboxGame final : public rw::core::GameLayer {
public:
    SandboxGame();

    void OnUpdate(float deltaSeconds, const rw::input::InputState& input) override;
    void OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window) override;
    std::string DebugTitle() const override;

private:
    void UpdateInteractionTarget(const rw::input::InputState& input);
    void UpdateDebugItemGrants(const rw::input::InputState& input);
    void GrantDebugItem(const std::string& itemId, int quantity);
    void GatherTargetNode(GatherableNode& node);
    void AddGatherableNode(const GatherableNode& node, const rw::math::Vec3& color);
    std::string InventorySummary() const;

    rw::scene::Scene m_scene;
    PlayerController m_player;
    ItemDatabase m_itemDatabase;
    Inventory m_inventory;
    Hotbar m_hotbar;
    std::vector<Interactable> m_interactables;
    std::vector<GatherableNode> m_gatherableNodes;
    const Interactable* m_currentTarget = nullptr;
    int m_currentGatherableIndex = -1;
    std::string m_lastInteractionMessage;
    std::string m_lastInventoryMessage;
    std::string m_lastGatherMessage;
};

} // namespace rw::game
