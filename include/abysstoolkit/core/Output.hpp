#pragma once

#include <abyssutils/signal/Signal.hpp>
#include <abyssutils/math/Vector2D.hpp>

namespace Abysstoolkit {
    class IOutput {
      public:
        virtual ~IOutput()           = default;
        virtual uint32_t    handle() = 0;
        virtual std::string port()   = 0;
        virtual std::string desc()   = 0;
        virtual uint32_t    fps()    = 0;

        struct {
            /* output removed */
            Abyssutils::Signal::CSignalT<> removed;
        } m_events;
    };
}
