#include "Engine/Input/InputState.h"

#include <cassert>

int main()
{
    rw::input::InputState input;

    assert(!input.QuitRequested());
    assert(!input.EscapeDown());

    input.SetEscapeDown(true);
    assert(input.EscapeDown());
    assert(input.QuitRequested());

    input.BeginFrame();
    assert(!input.EscapeDown());
    assert(input.QuitRequested());

    return 0;
}

