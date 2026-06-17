#pragma once

#include "Engine/Math/Vec3.h"
#include "Engine/Scene/Scene.h"
#include "Game/Source/AI/EnemyAgent.h"
#include "Game/Source/Interactable.h"
#include "Game/Source/Resources/GatherableNode.h"

#include <vector>

namespace rw::game {

struct BiomeLayout {
    rw::math::Vec3 playerSpawn { 0.0F, 1.8F, 6.0F };
    std::vector<rw::scene::SceneObject> sceneObjects;
    std::vector<Interactable> interactables;
    std::vector<GatherableNode> gatherableNodes;
    EnemyAgent realmWisp;
};

class BiomeLayoutBuilder {
public:
    static BiomeLayout BuildMistwoodHollow();
};

} // namespace rw::game
