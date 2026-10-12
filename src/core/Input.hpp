#pragma once

#include <cstdint>
#include <string>

#include <abysstoolkit/core/Input.hpp>

namespace Abysstoolkit::Input {
    eMouseButton buttonFromWayland(uint32_t wl);
}