#pragma once

namespace rw::input {

enum class Key {
    One,
    Two,
    Three,
    W,
    A,
    S,
    D,
    B,
    C,
    O,
    P,
    Q,
    R,
    Up,
    Down,
    Left,
    Right,
    Shift,
    E,
    Enter,
    Backspace,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    H,
    M,
    Escape,
    Count
};

class InputState {
public:
    void BeginFrame();

    void RequestQuit();
    bool QuitRequested() const;

    void SetEscapeDown(bool isDown);
    bool EscapeDown() const;
    void SetKeyDown(Key key, bool isDown);
    bool IsKeyDown(Key key) const;
    bool WasKeyPressed(Key key) const;
    void SetMousePosition(int x, int y);
    int MouseX() const;
    int MouseY() const;
    void AddMouseDelta(int deltaX, int deltaY);
    int MouseDeltaX() const;
    int MouseDeltaY() const;
    void SetMouseCaptureEnabled(bool enabled);
    bool MouseCaptureEnabled() const;

private:
    bool m_quitRequested = false;
    bool m_escapeDown = false;
    bool m_keys[static_cast<int>(Key::Count)] = {};
    bool m_pressed[static_cast<int>(Key::Count)] = {};
    int m_mouseX = 0;
    int m_mouseY = 0;
    int m_mouseDeltaX = 0;
    int m_mouseDeltaY = 0;
    bool m_mouseCaptureEnabled = false;
};

} // namespace rw::input
