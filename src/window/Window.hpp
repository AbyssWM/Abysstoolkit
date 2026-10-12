#pragma once

#include <abysstoolkit/window/Window.hpp>
#include <ext-session-lock-v1.hpp>

#include "../helpers/Memory.hpp"

namespace Abysstoolkit {
    struct SWindowCreationData {
        eWindowType                              type = HT_WINDOW_TOPLEVEL;
        std::optional<Abyssutils::Math::Vector2D> preferredSize;
        std::optional<Abyssutils::Math::Vector2D> minSize;
        std::optional<Abyssutils::Math::Vector2D> maxSize;
        std::string                              title             = "Abysstoolkit App";
        std::string                              class_            = "abysstoolkit-app";
        uint32_t                                 prefferedOutputId = 0;

        // popups
        Abyssutils::Math::Vector2D pos;
        SP<IWindow>               parent;

        // layers
        uint32_t                  layer         = 0;
        uint32_t                  anchor        = 0;
        int32_t                   exclusiveZone = 0;
        uint32_t                  exclusiveEdge = 0;
        Abyssutils::Math::Vector2D marginTopLeft, marginBottomRight;
        uint32_t                  kbInteractive = 0;
    };
};
