#pragma once

#include <abysstoolkit/core/EmbeddedBackend.hpp>
#include <abyssutils/signal/Listener.hpp>

#include "../helpers/Memory.hpp"

namespace Abysstoolkit {
    class CEmbeddedSurface;

    class CEmbeddedBackend final : public IEmbeddedBackend {
      public:
        explicit CEmbeddedBackend(const SCreationData& data);
        ~CEmbeddedBackend();

        void                     destroy() override;
        void                     setLogFn(LogFn&& fn) override;
        void                     addFd(int fd, std::function<void()>&& callback) override;
        void                     removeFd(int fd) override;
        SP<ISystemIconFactory>   systemIcons() override;
        ASP<CTimer>              addTimer(const TimerDuration& timeout, std::function<void(ASP<CTimer> self, void* data)> callback, void* data, bool force = false) override;
        void                     enterLoop() override;
        void                     addIdle(const std::function<void()>& callback) override;
        SP<CPalette>             getPalette() override;
        std::vector<SP<IOutput>> getOutputs() override;
        std::expected<SP<ISessionLockState>, eSessionLockError> aquireSessionLock() override;

        SP<IEmbeddedSurface>                                    createSurface() override;

        bool                                                    beginFrame();
        void                                                    endFrame();

      private:
        void                                   destroyNow();

        bool                                   m_destroyed        = false;
        bool                                   m_destroyRequested = false;
        size_t                                 m_activeFrames     = 0;
        std::vector<WP<CEmbeddedSurface>>      m_surfaces;
        Abyssutils::Signal::CAbyssSignalListener m_outputAddedListener;

        friend class IEmbeddedBackend;
    };
}
