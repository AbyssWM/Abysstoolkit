#pragma once

#include <abysstoolkit/element/Rectangle.hpp>

#include "../../core/AnimatedVariable.hpp"

namespace Abysstoolkit {

    struct SRectangleData {
        colorFn      color           = [] { return CAbyssColor{1.F, 1.F, 1.F, 1.F}; };
        int          rounding        = 0;
        colorFn      borderColor     = [] { return CAbyssColor{1.F, 1.F, 1.F, 1.F}; };
        int          borderThickness = 0;
        CDynamicSize size{CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
    };
    struct SRectangleImpl {
        SRectangleData         data;

        PHLANIMVAR<CAbyssColor> color;
        PHLANIMVAR<CAbyssColor> borderColor;

        WP<CRectangleElement>  self;
    };
}
