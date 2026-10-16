#pragma once

#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/signal/Signal.hpp>
#include <abyssutils/math/Vector2D.hpp>

#include "../core/Input.hpp"

namespace Abysstoolkit {
    class IElement;
    class IWindow;
    class IOutput;
    struct SWindowCreationData;

    enum eWindowType : uint8_t {
        HT_WINDOW_TOPLEVEL     = 0,
        HT_WINDOW_POPUP        = 1,
        HT_WINDOW_LAYER        = 2,
        HT_WINDOW_LOCK_SURFACE = 3,
    };

    enum eResizeEdge : uint32_t {
        HT_RESIZE_EDGE_NONE         = 0,
        HT_RESIZE_EDGE_TOP          = 1,
        HT_RESIZE_EDGE_BOTTOM       = 2,
        HT_RESIZE_EDGE_LEFT         = 4,
        HT_RESIZE_EDGE_TOP_LEFT     = 5,
        HT_RESIZE_EDGE_BOTTOM_LEFT  = 6,
        HT_RESIZE_EDGE_RIGHT        = 8,
        HT_RESIZE_EDGE_TOP_RIGHT    = 9,
        HT_RESIZE_EDGE_BOTTOM_RIGHT = 10,
    };

    class CWindowBuilder {
      public:
        ~CWindowBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CWindowBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        type(eWindowType);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        appTitle(std::string&&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        appClass(std::string&&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        preferredSize(const Abyssutils::Math::Vector2D&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        minSize(const Abyssutils::Math::Vector2D&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        maxSize(const Abyssutils::Math::Vector2D&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        resizable(bool);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder>        autosize(bool);

        // only for LAYER and LOCK_SURFACE
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> prefferedOutput(const Abyssutils::Memory::CSharedPointer<IOutput>& output);

        // only for HT_WINDOW_LAYER
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> marginTopLeft(const Abyssutils::Math::Vector2D&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> marginBottomRight(const Abyssutils::Math::Vector2D&);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> layer(uint32_t);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> anchor(uint32_t);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> exclusiveEdge(uint32_t);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> exclusiveZone(int32_t);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> kbInteractive(uint32_t);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> inhibitShortcuts(bool);

        // only for HT_WINDOW_POPUP
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> parent(const Abyssutils::Memory::CSharedPointer<IWindow>& parent);
        Abyssutils::Memory::CSharedPointer<CWindowBuilder> pos(const Abyssutils::Math::Vector2D&);

        Abyssutils::Memory::CSharedPointer<IWindow>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CWindowBuilder>        m_self;
        Abyssutils::Memory::CUniquePointer<SWindowCreationData> m_data;

        CWindowBuilder() = default;
    };

    class IWindow {
      public:
        virtual ~IWindow() = default;

        virtual Abyssutils::Math::Vector2D pixelSize() = 0;
        virtual float                     scale()     = 0;
        virtual void                      close()     = 0;
        virtual void                      open()      = 0;
        virtual Abyssutils::Math::Vector2D cursorPos() = 0;

        struct {
            // coordinates here are logical, meaning pixel size is this * scale()
            Abyssutils::Signal::CSignalT<Abyssutils::Math::Vector2D> resized;

            // user requested a close.
            Abyssutils::Signal::CSignalT<> closeRequest;

            // popup closed
            Abyssutils::Signal::CSignalT<> popupClosed;

            // layer closed
            Abyssutils::Signal::CSignalT<> layerClosed;

            // (global) key events
            Abyssutils::Signal::CSignalT<Input::SKeyboardKeyEvent> keyboardKey;
        } m_events;

        Abyssutils::Memory::CSharedPointer<IElement> m_rootElement;

        HT_HIDDEN : IWindow() = default;
    };
};
