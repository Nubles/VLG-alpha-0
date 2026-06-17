#pragma once

#include "Engine/Core/GameLayer.h"
#include "Engine/Scene/Scene.h"
#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Interactable.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/PlayerController.h"

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
    std::string InventorySummary() const;

    rw::scene::Scene m_scene;
    PlayerController m_player;
    ItemDatabase m_itemDatabase;
    Inventory m_inventory;
    Hotbar m_hotbar;
    std::vector<Interactable> m_interactables;
    const Interactable* m_currentTarget = nullptr;
    std::string m_lastInteractionMessage;
    std::string m_lastInventoryMessage;
};

} // namespace rw::game
