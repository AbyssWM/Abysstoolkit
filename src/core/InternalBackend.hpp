#pragma once

#include <abysstoolkit/core/Backend.hpp>
#include <abyssutils/os/FileDescriptor.hpp>
#include <abyssgraphics/resource/AsyncResourceGatherer.hpp>

#include "Backend.hpp"
#include "../helpers/Env.hpp"
#include "Logger.hpp"

namespace Abysstoolkit {

    class IBackend;
    class CBackend;
    class CPalette;
    class CConfigManager;
    class CSystemIconFactory;

    inline Abyssutils::Memory::CSharedPointer<IBackend>                             g_backend;
    inline Abyssutils::Memory::CWeakPointer<CBackend>                               g_waylandBackend;
    inline Abyssutils::Memory::CSharedPointer<Abyssgraphics::CAsyncResourceGatherer> g_asyncResourceGatherer;
    inline Abyssutils::Memory::CSharedPointer<CPalette>                             g_palette;
    inline Abyssutils::Memory::CSharedPointer<CConfigManager>                       g_config;
    inline Abyssutils::Memory::CSharedPointer<CSystemIconFactory>                   g_iconFactory;
}
