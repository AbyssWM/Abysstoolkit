#pragma once

#include <abysstoolkit/types/PointerShape.hpp>
#include <abysstoolkit/core/Input.hpp>
#include <abysstoolkit/core/Timer.hpp>
#include <abysstoolkit/core/SessionLock.hpp>

#include "../../helpers/Memory.hpp"

#include <vector>
#include <functional>

#include <xkbcommon/xkbcommon.h>
#include <xkbcommon/xkbcommon-compose.h>

#include <wayland-client.h>

#include <wayland.hpp>
#include <xdg-shell.hpp>
#include <linux-dmabuf-v1.hpp>
#include <fractional-scale-v1.hpp>
#include <viewporter.hpp>
#include <cursor-shape-v1.hpp>
#include <text-input-unstable-v3.hpp>
#include <wlr-layer-shell-unstable-v1.hpp>
#include <linux-drm-syncobj-v1.hpp>
#include <ext-session-lock-v1.hpp>
#include <keyboard-shortcuts-inhibit-unstable-v1.hpp>

#include <abyssal/allocator/GBM.hpp>
#include <abyssal/backend/Misc.hpp>

namespace Abysstoolkit {
    typedef std::function<void(void)> FIdleCallback;

    class CWaylandWindow;
    class IWaylandWindow;
    class CWaylandLayer;
    class CWaylandOutput;
    class CWaylandSessionLockState;

    class CWaylandPlatform {
      public:
        CWaylandPlatform() = default;
        ~CWaylandPlatform();

        bool                         attempt();

        void                         initSeat();
        void                         initShell();
        bool                         initDmabuf();
        void                         initIM();
        void                         initClipboard();
        void                         setCursor(ePointerShape shape);

        bool                         dispatchEvents();

        void                         setClipboard(const std::string& text);
        std::string                  readClipboard();

        SP<IWaylandWindow>           windowForSurf(wl_proxy* proxy);
        WP<CWaylandOutput>           outputForHandle(uint32_t handle);

        void                         onKey(uint32_t keycode, bool state);
        void                         onKeyboardEnter(wl_proxy* surf, wl_array* keys);
        void                         onKeyboardLeave();
        void                         resetKeyboardState();
        void                         startRepeatTimer();
        void                         stopRepeatTimer();

        void                         onRepeatTimerFire();

        SP<CWaylandSessionLockState> aquireSessionLock();

        // dmabuf formats
        std::vector<Abyssal::SDRMFormat> m_dmabufFormats;

        SP<Abyssal::CGBMAllocator>       m_allocator;

        struct {
            wl_display* display = nullptr;

            // hw-s types
            Abyssutils::Memory::CSharedPointer<CCWlRegistry>                           registry;
            Abyssutils::Memory::CSharedPointer<CCWlSeat>                               seat;
            Abyssutils::Memory::CSharedPointer<CCWlShm>                                shm;
            Abyssutils::Memory::CSharedPointer<CCXdgWmBase>                            xdg;
            Abyssutils::Memory::CSharedPointer<CCWlCompositor>                         compositor;
            Abyssutils::Memory::CSharedPointer<CCZwpLinuxDmabufV1>                     dmabuf;
            Abyssutils::Memory::CSharedPointer<CCZwpLinuxDmabufFeedbackV1>             dmabufFeedback;
            Abyssutils::Memory::CSharedPointer<CCWpFractionalScaleManagerV1>           fractional;
            Abyssutils::Memory::CSharedPointer<CCWpViewporter>                         viewporter;
            Abyssutils::Memory::CSharedPointer<CCWlKeyboard>                           keyboard;
            Abyssutils::Memory::CSharedPointer<CCWlPointer>                            pointer;
            Abyssutils::Memory::CSharedPointer<CCWpCursorShapeManagerV1>               cursorShapeMgr;
            Abyssutils::Memory::CSharedPointer<CCWpCursorShapeDeviceV1>                cursorShapeDev;
            Abyssutils::Memory::CSharedPointer<CCZwpTextInputManagerV3>                textInputManager;
            Abyssutils::Memory::CSharedPointer<CCZwpTextInputV3>                       textInput;
            Abyssutils::Memory::CSharedPointer<CCZwlrLayerShellV1>                     layerShell;
            Abyssutils::Memory::CSharedPointer<CCWpLinuxDrmSyncobjManagerV1>           syncobj;
            Abyssutils::Memory::CSharedPointer<CCExtSessionLockManagerV1>              sessionLock;
            Abyssutils::Memory::CSharedPointer<CCZwpKeyboardShortcutsInhibitManagerV1> shortcutsInhibitMgr;
            Abyssutils::Memory::CSharedPointer<CCWlDataDeviceManager>                  dataDeviceManager;
            Abyssutils::Memory::CSharedPointer<CCWlDataDevice>                         dataDevice;
            std::vector<Abyssutils::Memory::CSharedPointer<CCWlDataOffer>>             pendingOffers;
            Abyssutils::Memory::CSharedPointer<CCWlDataOffer>                          currentOffer;
            Abyssutils::Memory::CSharedPointer<CCWlDataSource>                         currentSource;
            std::string                                                               currentSourceText;

            // control
            bool initialized  = false;
            bool dmabufFailed = false;

            struct {
                xkb_context*             xkbContext      = nullptr;
                xkb_keymap*              xkbKeymap       = nullptr;
                xkb_state*               xkbState        = nullptr;
                xkb_compose_state*       xkbComposeState = nullptr;
                uint32_t                 currentLayer    = 0;
                uint32_t                 repeatRate = 10, repeatDelay = 500;
                std::vector<uint32_t>    pressedKeys;
                Input::SKeyboardKeyEvent repeatKeyEvent;
                ASP<CTimer>              repeatTimer;
            } seatState;

            struct {
                bool        entered = false, enabled = false;
                std::string preeditString;
                int         preeditBegin = 0, preeditEnd = 0;
                std::string commitString;
                size_t      deleteBefore = 0, deleteAfter = 0;

                std::string originalString;
            } imState;
        } m_waylandState;

        struct {
            int         fd       = -1;
            std::string nodeName = "";
        } m_drmState;

        std::vector<SP<CWaylandOutput>> m_outputs;

        std::vector<WP<CWaylandWindow>> m_windows;
        std::vector<WP<CWaylandLayer>>  m_layers;
        WP<IWaylandWindow>              m_currentWindow;       // window the pointer is over
        WP<IWaylandWindow>              m_keyboardWindow;      // window the compositor gave keyboard focus
        uint32_t                        m_currentMods     = 0; // HT modifiers, not xkb
        uint32_t                        m_lastEnterSerial = 0;
        // xdg_toplevel.resize requires the serial of a button PRESS, not release.
        uint32_t                     m_lastPointerButtonPressSerial = 0;

        WP<CWaylandSessionLockState> m_sessionLockState;
    };

    inline UP<CWaylandPlatform> g_waylandPlatform;
}
