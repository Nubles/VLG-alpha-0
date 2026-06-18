#include "Game/Source/Debug/DebugHelp.h"

namespace rw::game {

std::string DebugHelpText()
{
    return "Help: WASD move | Arrows/mouse look | M toggle mouse look | Shift sprint | E interact/gather/objective | "
           "C attack | B build | 1/2/3 buildables | Q/R rotate | Enter place | "
           "Backspace cancel build | F1-F5 grants | F6-F9 craft | F10 reset wisp | "
           "F11 craft realm_anchor | F12 reset progression | O save | P load | "
           "H help | Esc exit";
}

} // namespace rw::game
