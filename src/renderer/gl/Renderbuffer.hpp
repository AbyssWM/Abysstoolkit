#pragma once

#include "../../helpers/Memory.hpp"
#include "Framebuffer.hpp"
#include <abyssal/buffer/Buffer.hpp>
#include <abyssutils/signal/Listener.hpp>

namespace Abysstoolkit {

    class CSyncTimeline;
    class CEGLSync;
    class COpenGLRenderer;

    class CRenderbuffer {
      public:
        CRenderbuffer(COpenGLRenderer& renderer, SP<Aquamarine::IBuffer> buffer, uint32_t format);
        ~CRenderbuffer();

        bool                    good();
        void                    bind();
        void                    bindFB();
        void                    unbind();
        CFramebuffer*           getFB();
        uint32_t                getFormat();

        WP<Abyssal::IBuffer> m_hlBuffer;

        SP<CSyncTimeline>       m_syncTimeline;

      private:
        COpenGLRenderer& m_renderer;
        void*            m_image = nullptr;
        GLuint           m_rbo   = 0;
        CFramebuffer     m_framebuffer;
        uint32_t         m_drmFormat = 0;
        bool             m_good      = false;

        struct {
            Abyssutils::Signal::CAbyssSignalListener destroyBuffer;
        } m_listeners;
    };
}
