#pragma once

#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/memory/Atomic.hpp>
#include <abyssutils/cli/Logger.hpp>
#include <abyssal/backend/Null.hpp>
#include <abyssal/backend/Backend.hpp>
#include <functional>
#include <expected>
#include <string>

#include "LogTypes.hpp"
#include "BackendServices.hpp"
#include "SessionLock.hpp"
#include "../palette/Palette.hpp"

#include "CoreMacros.hpp"

namespace Abysstoolkit {
    class IWindow;
    class IOutput;
    class CTimer;
    class ISystemIconFactory;
    struct SWindowCreationData;

    class IBackend {
      public:
        virtual ~IBackend();

        using LogFn = std::function<void(eLogLevel, const std::string&)>;

        struct SBackendCreationData {
            explicit SBackendCreationData();

            Abyssutils::Memory::CSharedPointer<Abyssutils::CLI::CLoggerConnection> pLogConnection;
        };

        /*
            Create a backend.
            There can only be one backend per process: In case of another create(),
            it will fail.
        */
        static Abyssutils::Memory::CSharedPointer<IBackend> create();
        static Abyssutils::Memory::CSharedPointer<IBackend> createWithData(const SBackendCreationData& data);

        /*
            Destroy the backend.
            Backend will be destroyed once:
             - All refs YOU hold are dead
             - You call this fn
        */
        virtual void destroy() = 0;

        virtual void setLogFn(LogFn&& fn) = 0;

        /* These are non-owning. */
        virtual void addFd(int fd, std::function<void()>&& callback) = 0;
        virtual void removeFd(int fd)                                = 0;

        /*
            Get the system icon factory object,
            from which you can lookup icons.
        */
        virtual Abyssutils::Memory::CSharedPointer<ISystemIconFactory> systemIcons() = 0;

        /*
            Add a timer func. This will return a pointer, but the pointer doesn't need
            to be kept.
        */
        virtual Abyssutils::Memory::CAtomicSharedPointer<CTimer>
        addTimer(const TimerDuration& timeout, std::function<void(Abyssutils::Memory::CAtomicSharedPointer<CTimer> self, void* data)> cb_, void* data, bool force = false) = 0;

        /*
            Add an idle func. This fn will be executed as soon as possible, but
            after every pending event
        */
        virtual void addIdle(const std::function<void()>& fn) = 0;

        /*
            Enter the loop.
        */
        virtual void                                        enterLoop() = 0;

        virtual Abyssutils::Memory::CSharedPointer<CPalette> getPalette() = 0;

        /*
            Get currently registered outputs.
            Make sure you register the `removed` event to get rid of your reference once the output is removed.
        */
        virtual std::vector<Abyssutils::Memory::CSharedPointer<IOutput>> getOutputs() = 0;

        /*
            Create and lock the graphical session.
            It is required to call this before HT_WINDOW_LOCK_SURFACE can be used.
        */
        virtual std::expected<Abyssutils::Memory::CSharedPointer<ISessionLockState>, eSessionLockError> aquireSessionLock() = 0;

        struct {
            /*
                Get notified when a new output was added.
            */
            Abyssutils::Signal::CSignalT<Abyssutils::Memory::CSharedPointer<IOutput>> outputAdded;
        } m_events;

      protected:
        IBackend();
    };
};
