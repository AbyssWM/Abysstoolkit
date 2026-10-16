#pragma once

#include "Backend.hpp"
#include "BackendServices.hpp"

namespace Abysstoolkit {
    class IEmbeddedSurface;

    class IEmbeddedBackend : public IBackend {
      public:
        struct SCreationData {
            SBackendCreationData                                    common;

            Abyssutils::Memory::CSharedPointer<IEventLoop>           eventLoop;
            Abyssutils::Memory::CSharedPointer<IClipboard>           clipboard;
            Abyssutils::Memory::CSharedPointer<ITextInput>           textInput;
            Abyssutils::Memory::CSharedPointer<ICursor>              cursor;
            Abyssutils::Memory::CSharedPointer<IOutputProvider>      outputs;
            Abyssutils::Memory::CSharedPointer<ISessionLockProvider> sessionLock;
        };

        // Creation and destruction require the GLES3 EGL context that will be
        // used for rendering to be current on the calling thread.
        static Abyssutils::Memory::CSharedPointer<IEmbeddedBackend> create(const SCreationData& data);

        // Embedded surfaces do not support native popups yet. Popup-backed
        // controls, including combobox dropdowns and tooltips, remain closed.
        virtual Abyssutils::Memory::CSharedPointer<IEmbeddedSurface> createSurface() = 0;

      protected:
        IEmbeddedBackend() = default;
    };
}
