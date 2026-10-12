#include <abysstoolkit/palette/Palette.hpp>

#include "ConfigManager.hpp"

#include "../helpers/Memory.hpp"
#include "../core/InternalBackend.hpp"

using namespace Abysstoolkit;

SP<CPalette> CPalette::palette() {
    auto x        = g_config->getPalette();
    x->m_isConfig = true;
    return x;
}

SP<CPalette> CPalette::emptyPalette() {
    return SP<CPalette>(new CPalette());
}
