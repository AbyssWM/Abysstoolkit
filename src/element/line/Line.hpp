#pragma once

#include <abysstoolkit/element/Line.hpp>

#include "../../helpers/Memory.hpp"

namespace Abysstoolkit {

    struct SLineData {
        colorFn                                color = [] { return CAbyssColor{1.F, 1.F, 1.F, 1.F}; };
        int                                    thick = 2;
        std::vector<Abyssutils::Math::Vector2D> points;
        CDynamicSize                           size{CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
    };
    struct SLineImpl {
        SLineData        data;

        WP<CLineElement> self;
    };
}
