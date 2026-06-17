#pragma once

#include "Engine/Math/Transform.h"
#include "Engine/Scene/Camera.h"
#include "Game/Source/Building/BuildPlacementResult.h"

#include <string>
#include <vector>

namespace rw::game {

class BuildableDatabase;
class Inventory;
class ItemDatabase;

class BuildPlacementController {
public:
    void ToggleBuildMode();
    void CancelBuildMode();
    void SelectBuildable(const std::string& buildableId);
    void Rotate(float yawDeltaRadians);
    void UpdatePreview(const BuildableDatabase& buildables, const rw::scene::Camera& camera);

    BuildPlacementResult TryPlace(
        const BuildableDatabase& buildables,
        const ItemDatabase& items,
        Inventory& inventory,
        const std::vector<PlacedBuildable>& placedBuildables);

    bool BuildModeActive() const;
    const std::string& SelectedBuildableId() const;
    float YawRadians() const;
    const rw::math::Transform& PreviewTransform() const;

private:
    bool HasCost(const BuildableDatabase& buildables, const Inventory& inventory) const;
    bool IsPreviewPlacementValid(const rw::math::Vec3& cameraPosition) const;
    bool OverlapsExisting(
        const BuildableDatabase& buildables,
        const std::vector<PlacedBuildable>& placedBuildables) const;

    bool m_buildModeActive = false;
    std::string m_selectedBuildableId;
    float m_yawRadians = 0.0F;
    rw::math::Transform m_previewTransform;
    rw::math::Vec3 m_lastCameraPosition;
};

} // namespace rw::game
