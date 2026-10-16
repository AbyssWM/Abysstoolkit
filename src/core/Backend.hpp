#pragma once

#include <abysstoolkit/core/Backend.hpp>
#include <abysstoolkit/core/BackendServices.hpp>
#include <abyssutils/eventLoop/EventLoop.hpp>
#include <abyssutils/os/FileDescriptor.hpp>
#include <abyssutils/cli/Logger.hpp>
#include <abyssgraphics/resource/AsyncResourceGatherer.hpp>

#include <atomic>
#include <mutex>
#include <thread>

#include "../helpers/Env.hpp"
#include "../helpers/Memory.hpp"

namespace Abysstoolkit {

    class CPalette;
    class CConfigManager;
    class CSystemIconFactory;

    class CBackend : public IBackend, public IEventLoop {
      public:
        CBackend();
        virtual ~CBackend();

        virtual void                     destroy();
        virtual void                     setLogFn(LogFn&& fn);
        virtual void                     addFd(int fd, std::function<void()>&& callback);
        virtual void                     removeFd(int fd);
        virtual SP<ISystemIconFactory>   systemIcons();
        virtual ASP<CTimer>              addTimer(const TimerDuration& timeout, std::function<void(ASP<CTimer> self, void* data)> cb_, void* data, bool force = false);
        virtual void                     addIdle(const std::function<void()>& fn);
        virtual void                     cancelPending();
        virtual void                     enterLoop();
        virtual std::vector<SP<IOutput>> getOutputs();
        virtual SP<CPalette>             getPalette();
        virtual std::expected<SP<ISessionLockState>, eSessionLockError> aquireSessionLock();

        // ======================= Internal fns ======================= //

        void terminate();
        void reloadTheme();
        void updateTimer(CTimer* timer, const std::chrono::steady_clock::time_point& expires);
        void cancelTimer(CTimer* timer);

        // schedule function to when fd is readable (WL_EVENT_READABLE / POLLIN),
        // takes ownership of fd
        void        doOnReadable(Abyssutils::OS::CFileDescriptor fd, std::function<void()>&& fn);

        SP<IWindow> openWindow(const SWindowCreationData& data);

        //

        Abyssutils::Memory::CSharedPointer<Abyssal::CBackend> m_aqBackend;

        std::atomic<bool>                                       m_terminate = false;
        std::atomic<bool>                                       m_cleaned   = false;
        std::mutex                                              m_loopStateMutex;
        bool                                                    m_loopRunning = false;
        std::thread::id                                         m_loopThread;

        struct SFDListener {
            int                                                                fd = -1;
            Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::IFDSource> source;
        };

        struct STimer {
            Abyssutils::Memory::CAtomicSharedPointer<CTimer>                 timer;
            Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::ITimer> loopTimer;
        };

        bool initializeEventLoop();
        void dispatchWayland(Abyssutils::EventLoop::IFDSource& source, Abyssutils::EventLoop::FdEventMask events);
        void flushWayland(Abyssutils::EventLoop::IFDSource& source);
        void cleanup();
        void registerTimer(const Abyssutils::Memory::CAtomicSharedPointer<CTimer>& timer, const std::chrono::steady_clock::time_point& expires);
        void removeTimer(CTimer* timer);

        Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::IEventLoop>          m_eventLoop;
        Abyssutils::Memory::CAtomicSharedPointer<Abyssutils::EventLoop::ILoopExecutor> m_eventLoopExecutor;
        Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::IFDSource>           m_waylandSource;
        Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::IFDSource>           m_configSource;
        Abyssutils::Memory::CSharedPointer<Abyssutils::EventLoop::IPostDispatchHook>   m_waylandPostDispatch;
        bool                                                                         m_waylandWantsWrite = false;
        std::atomic<uint64_t>                                                        m_pendingGeneration = 0;

        std::vector<SFDListener>                                                     m_userFds;
        std::vector<STimer>                                                          m_timers;
    };
}
