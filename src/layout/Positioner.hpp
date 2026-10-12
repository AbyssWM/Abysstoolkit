#pragma once

#include <abyssutils/math/Box.hpp>
#include <abyssutils/math/Vector2D.hpp>
#include "../helpers/Memory.hpp"

namespace Abysstoolkit {
    class IElement;

    struct SPositionerData {
        Abyssutils::Math::CBox baseBox;
    };

    struct SRepositionData {
        Abyssutils::Math::Vector2D offset = {0, 0};
        bool                      growX  = false;
        bool                      growY  = false;
    };

    class CPositioner {
      public:
        void position(SP<IElement> element, const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        void positionChildren(SP<IElement> element, const SRepositionData& data = {});
        void repositionNeeded(SP<IElement> element, bool force = false);

      private:
        void   initElementIfNeeded(SP<IElement> element);

        size_t m_depth = 0;
    };

    inline UP<CPositioner> g_positioner = makeUnique<CPositioner>();
}