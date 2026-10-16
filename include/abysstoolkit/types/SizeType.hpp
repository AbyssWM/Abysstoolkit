#pragma once

#include <cstdint>
#include <abyssutils/math/Vector2D.hpp>

#include "../core/CoreMacros.hpp"

namespace Abysstoolkit {
    class CDynamicSize {
      public:
        enum eSizingType : uint8_t {
            HT_SIZE_ABSOLUTE,
            HT_SIZE_PERCENT,
            HT_SIZE_AUTO, // contain child(ren)
        };

        CDynamicSize(eSizingType typeX, eSizingType typeY, const Abyssutils::Math::Vector2D& size);

        Abyssutils::Math::Vector2D calculate(Abyssutils::Math::Vector2D elSize) const;

        AT_HIDDEN :

            bool
            hasAuto();

      private:
        eSizingType               m_typeX = HT_SIZE_ABSOLUTE, m_typeY = HT_SIZE_ABSOLUTE;
        Abyssutils::Math::Vector2D m_value;
    };
}