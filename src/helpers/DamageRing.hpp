#pragma once

#include <abyssutils/math/Region.hpp>
#include <array>

namespace Abysstoolkit {
    constexpr static int DAMAGE_RING_PREVIOUS_LEN = 2;

    class CDamageRing {
      public:
        void                     setSize(const Abyssutils::Math::Vector2D& size_);
        bool                     damage(Abyssutils::Math::CRegion&& rg);
        void                     damageEntire();
        void                     rotate();
        Abyssutils::Math::CRegion getBufferDamage(int age);
        bool                     hasChanged();

      private:
        Abyssutils::Math::Vector2D                                      m_size;
        Abyssutils::Math::CRegion                                       m_current;
        std::array<Abyssutils::Math::CRegion, DAMAGE_RING_PREVIOUS_LEN> m_previous;
        size_t                                                         m_previousIdx = 0;
    };
}
