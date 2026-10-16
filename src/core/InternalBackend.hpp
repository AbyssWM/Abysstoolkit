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

    inline Hyprutils::Memory::CSharedPointer<IBackend>                             g_backend;
    inline Hyprutils::Memory::CWeakPointer<CBackend>                               g_waylandBackend;
    inline Hyprutils::Memory::CSharedPointer<Hyprgraphics::CAsyncResourceGatherer> g_asyncResourceGatherer;
    inline Hyprutils::Memory::CSharedPointer<CPalette>                             g_palette;
    inline Hyprutils::Memory::CSharedPointer<CConfigManager>                       g_config;
    inline Hyprutils::Memory::CSharedPointer<CSystemIconFactory>                   g_iconFactory;
}
