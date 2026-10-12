#pragma once

#include "OpenGL.hpp"
#include "../../helpers/Memory.hpp"
#include <abyssutils/os/FileDescriptor.hpp>

namespace Abysstoolkit {
    class CEGLSync {
      public:
        static UP<CEGLSync> create();

        ~CEGLSync();

        Abyssutils::OS::CFileDescriptor&  fd();
        Abyssutils::OS::CFileDescriptor&& takeFd();
        bool                             isValid();

      private:
        CEGLSync() = default;

        Abyssutils::OS::CFileDescriptor m_fd;
        EGLSyncKHR                     m_sync  = EGL_NO_SYNC_KHR;
        bool                           m_valid = false;

        friend class CAbyssOpenGLImpl;
    };
}
