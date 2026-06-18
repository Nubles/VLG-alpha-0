#pragma once

#include "Engine/Core/GameLayer.h"
#include "Engine/Scene/Scene.h"
#include "Game/Source/AI/EnemyAgent.h"
#include "Game/Source/Biomes/BiomeDatabase.h"
#include "Game/Source/Building/BuildableDatabase.h"
#include "Game/Source/Building/BuildPlacementController.h"
#include "Game/Source/Combat/PlayerCombat.h"
#include "Game/Source/Crafting/RecipeDatabase.h"
#include "Game/Source/Debug/DebugMessageLog.h"
#include "Game/Source/Inventory/Hotbar.h"
#include "Game/Source/Inventory/Inventory.h"
#include "Game/Source/Interactable.h"
#include "Game/Source/Items/ItemDatabase.h"
#include "Game/Source/Objectives/ObjectiveState.h"
#include "Game/Source/Progression/ProgressionState.h"
#include "Game/Source/PlayerController.h"
#include "Game/Source/Resources/GatherableNode.h"
#include "Game/Source/Runtime/SandboxWorldState.h"
#include "Game/Source/SaveLoad/SaveData.h"
#include "Game/Source/UI/HudState.h"
#include "Game/Source/UI/VerticalSliceHud.h"

#include <string>
#include <vector>

namespace rw::game {

class SandboxGame final : public rw::core::GameLayer {
public:
    SandboxGame();

    void OnUpdate(float deltaSeconds, rw::input::InputState& input) override;
    void OnRender(rw::renderer::Renderer& renderer, rw::platform::Window& window) override;
    std::string DebugTitle() const override;

private:
    void UpdateInteractionTarget(const rw::input::InputState& input);
    void UpdateDebugItemGrants(const rw::input::InputState& input);
    void UpdateDebugCrafting(const rw::input::InputState& input);
    void UpdateBuildPlacement(const rw::input::InputState& input);
    void UpdateCombatAndEnemies(float deltaSeconds, const rw::input::InputState& input);
    void UpdateSaveLoad(const rw::input::InputState& input);
    void UpdateDebugHelp(const rw::input::InputState& input);
    void UpdateMouseLook(rw::input::InputState& input);
    void GrantDebugItem(const std::string& itemId, int quantity);
    void CraftDebugRecipe(const std::string& recipeId);
    void InteractWithRealmFracture();
    void ResetProgressionState();
    void UpdateRealmFractureVisual();
    void ResetRealmWisp();
    void SyncRealmWispVisual();
    void LoadMistwoodHollow();
    SaveData CaptureSaveData() const;
    void ApplySaveData(const SaveData& data);
    void SyncBuildPreview();
    void AddPlacedBuildable(const PlacedBuildable& placed);
    std::string SelectedBuildableName() const;
    std::string TargetName() const;
    std::string ObjectiveStatus() const;
    HudState BuildHudState() const;
    void PushDebugMessage(const std::string& message);
    void GatherTargetNode(GatherableNode& node);
    void AddGatherableNode(const GatherableNode& node, const rw::math::Vec3& color);
    std::string InventorySummary() const;

    rw::scene::Scene m_scene;
    PlayerController m_player;
    ItemDatabase m_itemDatabase;
    RecipeDatabase m_recipeDatabase;
    BiomeDatabase m_biomeDatabase;
    const BiomeDefinition* m_currentBiome = nullptr;
    BuildableDatabase m_buildableDatabase;
    BuildPlacementController m_buildPlacement;
    PlayerCombat m_playerCombat;
    ProgressionState m_progression;
    ObjectiveState m_objectiveState;
    DebugMessageLog m_messages { 4 };
    SandboxWorldState m_worldState;
    Inventory m_inventory;
    Hotbar m_hotbar;
    VerticalSliceHud m_hud;
    bool m_playerMouseLookEnabled = false;
    std::vector<Interactable> m_interactables;
    std::vector<GatherableNode> m_gatherableNodes;
    EnemyAgent m_realmWisp;
    const Interactable* m_currentTarget = nullptr;
    int m_currentGatherableIndex = -1;
    std::string m_lastInteractionMessage;
    std::string m_lastInventoryMessage;
    std::string m_lastGatherMessage;
    std::string m_lastCraftMessage;
    std::string m_lastBuildMessage;
    std::string m_lastCombatMessage;
    std::string m_lastProgressionMessage;
    std::string m_lastSaveMessage;
    std::string m_lastInputMessage;
};

} // namespace rw::game
