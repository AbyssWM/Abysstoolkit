#pragma once

#include <functional>

#include <abyssutils/os/FileDescriptor.hpp>

#include <abysstoolkit/core/BackendServices.hpp>

#include "../helpers/Memory.hpp"

namespace Abysstoolkit {
    class IWindow;
    struct SWindowCreationData;

    struct SBackendLifetime {
        SBackendLifetime();

        const uint64_t generation;
    };

    struct SBackendServices {
        SP<SBackendLifetime>                                                         lifetime = makeShared<SBackendLifetime>();
        SP<IEventLoop>                                                               eventLoop;
        SP<IClipboard>                                                               clipboard;
        SP<ITextInput>                                                               textInput;
        SP<ICursor>                                                                  cursor;
        SP<IOutputProvider>                                                          outputs;
        SP<ISessionLockProvider>                                                     sessionLock;

        std::function<SP<IWindow>(const SWindowCreationData&)>                       openWindow;
        std::function<void(Abyssutils::OS::CFileDescriptor, std::function<void()>&&)> doOnReadable;
    };

    SBackendServices            makeDefaultBackendServices();

    inline UP<SBackendServices> g_backendServices;
}
