#pragma once

#include "Engine/Math/Transform.h"

#include <string>
#include <vector>

namespace rw::scene {

enum class DebugPrimitive {
    Grid,
    Cube
};

struct SceneObject {
    std::string name;
    rw::math::Transform transform;
    DebugPrimitive primitive = DebugPrimitive::Cube;
    rw::math::Vec3 color { 0.8F, 0.8F, 0.8F };
};

class Scene {
public:
    void AddObject(SceneObject object);
    const std::vector<SceneObject>& Objects() const;

private:
    std::vector<SceneObject> m_objects;
};

} // namespace rw::scene
