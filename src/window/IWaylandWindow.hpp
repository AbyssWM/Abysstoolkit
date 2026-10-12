#pragma once

#include <abysstoolkit/window/Window.hpp>

#include <abyssutils/math/Vector2D.hpp>

#include <abyssal/allocator/Swapchain.hpp>
#include <abyssal/allocator/GBM.hpp>

#include "../helpers/Memory.hpp"
#include "ToolkitWindow.hpp"

#include <wayland.hpp>
#include <xdg-shell.hpp>
#include <fractional-scale-v1.hpp>
#include <viewporter.hpp>
#include <text-input-unstable-v3.hpp>
#include <linux-drm-syncobj-v1.hpp>

#include <chrono>

namespace Abysstoolkit {
    class CSyncTimeline;

    class CWaylandBuffer {
      public:
        CWaylandBuffer(SP<Abyssal::IBuffer> buffer);
        ~CWaylandBuffer();
        bool              good();

        bool              m_pendingRelease = false;
        SP<CSyncTimeline> m_timeline;
        bool              m_firstTimeIgnoreSync = true;

        struct {
            SP<CCWlBuffer>                    buffer;
            SP<CCWpLinuxDrmSyncobjTimelineV1> syncTimeline;
        } m_waylandState;

        Abyssutils::Memory::CWeakPointer<Abyssal::IBuffer> m_buffer;
    };

    class IWaylandWindow : public IToolkitWindow {
      public:
        virtual ~IWaylandWindow() = default;

        virtual Abyssutils::Math::Vector2D pixelSize();
        virtual float                     scale();
        virtual void                      render();
        virtual void                      setCursor(ePointerShape shape);
        virtual SP<IWindow>               openPopup(const SWindowCreationData& data);
        virtual void                      mouseMove(const Abyssutils::Math::Vector2D& local);
        virtual void                      mouseButton(const Input::eMouseButton button, bool state);
        virtual void                      mouseAxis(const Input::eAxisAxis axis, float delta);
        virtual void                      setIMTo(const Abyssutils::Math::CBox& box, const std::string& str, size_t cursor);
        virtual void                      resetIM();
        virtual void                      onPreRender();

        std::vector<WP<IWindow>>          m_popups;

      protected:
        virtual void onCallback();
        virtual void onScaleUpdate();
        virtual void configure(const Abyssutils::Math::Vector2D& size, uint32_t serial);
        virtual void resizeSwapchain(const Abyssutils::Math::Vector2D& pixelSize);

        void         prepareExplicit(SP<CWaylandBuffer>);
        void         submitExplicit(SP<CWaylandBuffer>);

        float        m_fractionalScale = 1.0;

        bool         m_open                  = false;
        bool         m_ignoreNextButtonEvent = false;

        struct {
            SP<CCWlSurface>                         surface;
            SP<CCXdgSurface>                        xdgSurface;
            SP<CCXdgToplevel>                       xdgToplevel;
            SP<CCWlCallback>                        frameCallback;
            SP<CCWpLinuxDrmSyncobjSurfaceV1>        syncobjSurf;

            std::array<SP<CWaylandBuffer>, 2>       wlBuffers;
            SP<Abyssal::CSwapchain>              swapchain;
            size_t                                  bufIdx = 0;

            Abyssutils::Math::Vector2D               size;
            Abyssutils::Math::Vector2D               logicalSize;
            float                                   appliedScale;
            SP<CCWpFractionalScaleV1>               fractional = nullptr;
            SP<CCWpViewport>                        viewport   = nullptr;
            uint32_t                                serial     = 0;

            std::optional<Abyssutils::Math::CRegion> lastOpaqueRegion;
        } m_waylandState;

        std::chrono::steady_clock::time_point m_lastFrame = std::chrono::steady_clock::now();

        friend class CWaylandPlatform;
        friend class CWaylandPopup;
        friend class CWaylandWindow;
    };

}