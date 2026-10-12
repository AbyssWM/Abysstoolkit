#pragma once

#include <abysstoolkit/element/Element.hpp>
#include <abyssutils/signal/Signal.hpp>

#include <functional>

#include "../helpers/Memory.hpp"
#include "../core/Input.hpp"

#include <abyssutils/math/Box.hpp>

namespace Abysstoolkit {
    class IToolkitWindow;
    struct SPositionerData;
    struct SToolkitWindowData;
    class CDynamicSize;

    struct SElementInternalData {
        Abyssutils::Memory::CWeakPointer<IElement>                self;
        Abyssutils::Memory::CWeakPointer<IToolkitWindow>          window;
        Abyssutils::Math::CBox                                    position;

        UP<SPositionerData>                                      positionerData;
        UP<SToolkitWindowData>                                   toolkitWindowData;

        std::vector<Abyssutils::Memory::CSharedPointer<IElement>> children;

        IElement::ePositionMode                                  positionMode  = IElement::HT_POSITION_AUTO;
        uint8_t                                                  positionFlags = 0;
        Abyssutils::Math::Vector2D                                absoluteOffset;
        bool                                                     growV                   = false;
        bool                                                     growH                   = false;
        float                                                    margin                  = 0;
        bool                                                     userRequestedMouseInput = false;
        bool                                                     grouped                 = false;

        // tooltip
        std::string tooltip    = "";
        bool        hasTooltip = false;

        // rendering: clip children to parent box
        bool         clipChildren = false;

        WP<IElement> parent;

        bool         failedPositioning = false;

        struct {
            Abyssutils::Signal::CSignalT<Abyssutils::Math::Vector2D> mouseEnter; // local coords
            Abyssutils::Signal::CSignalT<Abyssutils::Math::Vector2D> mouseMove;  // local coords
            Abyssutils::Signal::CSignalT<Input::eMouseButton, bool> mouseButton;
            Abyssutils::Signal::CSignalT<>                          mouseLeave;
            Abyssutils::Signal::CSignalT<Input::eAxisAxis, float>   mouseAxis;
            Abyssutils::Signal::CSignalT<Input::SKeyboardKeyEvent>  key;
            Abyssutils::Signal::CSignalT<>                          keyboardEnter;
            Abyssutils::Signal::CSignalT<>                          keyboardLeave;
        } m_externalEvents;

        struct {
            std::function<void(const Abyssutils::Math::Vector2D&)> mouseEnter;
            std::function<void()>                                 mouseLeave;
            std::function<void(const Abyssutils::Math::Vector2D&)> mouseMove;
            std::function<void(Input::eMouseButton, bool)>        mouseButton;
            std::function<void(Input::eAxisAxis, float)>          mouseAxis;
            std::function<void()>                                 repositioned;
        } userFns;

        //
        void                      bfHelper(std::vector<SP<IElement>> elements, const std::function<void(SP<IElement>)>& fn);
        void                      breadthfirst(const std::function<void(SP<IElement>)>& fn);
        void                      setWindow(SP<IToolkitWindow> w);
        void                      damageEntire();
        void                      setPosition(const Abyssutils::Math::CBox& box);
        void                      setFailedPositioning(bool set);
        Abyssutils::Math::Vector2D maxChildSize(const Abyssutils::Math::Vector2D& parent);
        Abyssutils::Math::Vector2D getPreferredSizeGeneric(const CDynamicSize& size, const Abyssutils::Math::Vector2D& parent);
    };

}
