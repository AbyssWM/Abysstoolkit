#pragma once

#include "Color.hpp"

#include <functional>
#include <vector>

namespace Abysstoolkit {

    struct CGradientValueData {
        std::vector<CAbyssColor> m_vColors;
        float                   m_fAngle = 0.F;

        CGradientValueData() = default;
        CGradientValueData(const CAbyssColor& col) : m_vColors{col} {}

        bool operator==(const CGradientValueData& rhs) const;
    };

    using gradientFn = std::function<CGradientValueData()>;
}
