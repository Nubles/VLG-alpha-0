#pragma once

namespace rw::input {

enum class Key {
    W,
    A,
    S,
    D,
    Up,
    Down,
    Left,
    Right,
    Shift,
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

private:
    bool m_quitRequested = false;
    bool m_escapeDown = false;
    bool m_keys[static_cast<int>(Key::Count)] = {};
};

} // namespace rw::input
