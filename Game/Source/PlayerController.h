#pragma once

#include "Engine/Input/InputState.h"
#include "Engine/Scene/Camera.h"
#include "Game/Source/PlayerVitals.h"

namespace rw::game {

class PlayerController {
public:
    PlayerController();

    void Update(float deltaSeconds, const rw::input::InputState& input);

    const rw::scene::Camera& Camera() const;
    void SetPosition(const rw::math::Vec3& position);
    void SetLook(float yawRadians, float pitchRadians);
    const PlayerVitals& Vitals() const;
    PlayerVitals& Vitals();

private:
    rw::scene::Camera m_camera;
    PlayerVitals m_vitals;
};

} // namespace rw::game
