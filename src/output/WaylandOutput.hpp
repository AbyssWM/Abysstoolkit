#pragma once

#include "wayland.hpp"

#include <cstdint>
#include <abysstoolkit/core/Output.hpp>

#include <abyssutils/math/Misc.hpp>
#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/memory/WeakPtr.hpp>
#include <abyssutils/math/Vector2D.hpp>

namespace Abysstoolkit {
    class CWaylandOutput : public IOutput {
      public:
        CWaylandOutput(wl_proxy* wlResource, uint32_t id);
        ~CWaylandOutput() = default;

        virtual uint32_t                              handle();
        virtual std::string                           port();
        virtual std::string                           desc();
        virtual uint32_t                              fps();

        uint32_t                                      m_id       = 0;
        bool                                          m_focused  = false;
        Abyssutils::Memory::CSharedPointer<CCWlOutput> m_wlOutput = nullptr;

        struct {
            bool                        done      = false;
            Abyssutils::Math::eTransform transform = Abyssutils::Math::ABYSSUTILS_TRANSFORM_NORMAL;
            Abyssutils::Math::Vector2D   size;
            uint32_t                    fps   = 60;
            uint32_t                    scale = 1;
            std::string                 name  = "";
            std::string                 port  = "";
            std::string                 desc  = "";
        } m_configuration;
    };
}
