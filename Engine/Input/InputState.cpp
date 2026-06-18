#include "Engine/Input/InputState.h"

namespace rw::input {

void InputState::BeginFrame()
{
    m_escapeDown = false;
    for (bool& pressed : m_pressed) {
        pressed = false;
    }
    m_mouseDeltaX = 0;
    m_mouseDeltaY = 0;
}

void InputState::RequestQuit()
{
    m_quitRequested = true;
}

bool InputState::QuitRequested() const
{
    return m_quitRequested;
}

void InputState::SetEscapeDown(bool isDown)
{
    m_escapeDown = isDown;
    SetKeyDown(Key::Escape, isDown);
    if (isDown) {
        RequestQuit();
    }
}

bool InputState::EscapeDown() const
{
    return m_escapeDown;
}

void InputState::SetKeyDown(Key key, bool isDown)
{
    const int index = static_cast<int>(key);
    m_pressed[index] = isDown && !m_keys[index];
    m_keys[index] = isDown;
}

bool InputState::IsKeyDown(Key key) const
{
    return m_keys[static_cast<int>(key)];
}

bool InputState::WasKeyPressed(Key key) const
{
    return m_pressed[static_cast<int>(key)];
}

void InputState::SetMousePosition(int x, int y)
{
    m_mouseX = x;
    m_mouseY = y;
}

int InputState::MouseX() const
{
    return m_mouseX;
}

int InputState::MouseY() const
{
    return m_mouseY;
}

void InputState::AddMouseDelta(int deltaX, int deltaY)
{
    m_mouseDeltaX += deltaX;
    m_mouseDeltaY += deltaY;
}

int InputState::MouseDeltaX() const
{
    return m_mouseDeltaX;
}

int InputState::MouseDeltaY() const
{
    return m_mouseDeltaY;
}

void InputState::SetMouseCaptureEnabled(bool enabled)
{
    m_mouseCaptureEnabled = enabled;
}

bool InputState::MouseCaptureEnabled() const
{
    return m_mouseCaptureEnabled;
}

} // namespace rw::input
