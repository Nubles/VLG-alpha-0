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

private:
    bool m_quitRequested = false;
    bool m_escapeDown = false;
    bool m_keys[static_cast<int>(Key::Count)] = {};
    bool m_pressed[static_cast<int>(Key::Count)] = {};
};

} // namespace rw::input
