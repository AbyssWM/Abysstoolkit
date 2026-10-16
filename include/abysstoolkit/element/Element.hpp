#pragma once

#include <vector>
#include <cstdint>
#include <optional>
#include <functional>

#include <abyssutils/memory/SharedPtr.hpp>
#include <abyssutils/memory/WeakPtr.hpp>
#include <abyssutils/memory/UniquePtr.hpp>
#include <abyssutils/math/Box.hpp>

#include "../types/PointerShape.hpp"
#include "../palette/Color.hpp"
#include "../core/Input.hpp"
#include "../core/Animation.hpp"
#include "../core/CoreMacros.hpp"

namespace Abysstoolkit {

    struct SElementInternalData;

    using colorFn = std::function<CAbyssColor()>;

    class IElement {
      public:
        enum ePositionMode : uint8_t {
            HT_POSITION_ABSOLUTE = 0,
            HT_POSITION_AUTO,
        };

        enum ePositionFlag : uint8_t {
            HT_POSITION_FLAG_HCENTER = (1 << 0),
            HT_POSITION_FLAG_VCENTER = (1 << 1),
            HT_POSITION_FLAG_CENTER  = HT_POSITION_FLAG_HCENTER | HT_POSITION_FLAG_VCENTER,
            HT_POSITION_FLAG_LEFT    = (1 << 2),
            HT_POSITION_FLAG_RIGHT   = (1 << 3),
            HT_POSITION_FLAG_TOP     = (1 << 4),
            HT_POSITION_FLAG_BOTTOM  = (1 << 5),

            HT_POSITION_FLAG_ALL = 0xFF,
        };

        virtual ~IElement();

        virtual void                      paint() = 0;
        virtual Abyssutils::Math::Vector2D size()  = 0;
        virtual Abyssutils::Math::Vector2D posFromParent();
        virtual void                      reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});

        // TODO: move this to builders, this is clunky
        virtual void setPositionMode(ePositionMode mode);
        virtual void setPositionFlag(ePositionFlag flag, bool set);
        virtual void setAbsolutePosition(const Abyssutils::Math::Vector2D& offset);
        virtual void addChild(Abyssutils::Memory::CSharedPointer<IElement> child);
        virtual void removeChild(Abyssutils::Memory::CSharedPointer<IElement> child);
        virtual void clearChildren();
        virtual void setMargin(float thick);
        virtual void setGrouped(bool grouped);

        // this will make this element get mouse input, then you can get events
        virtual void setReceivesMouse(bool x);
        virtual void setMouseEnter(std::function<void(const Abyssutils::Math::Vector2D&)>&& fn);
        virtual void setMouseLeave(std::function<void()>&& fn);
        virtual void setMouseMove(std::function<void(const Abyssutils::Math::Vector2D&)>&& fn);
        virtual void setMouseButton(std::function<void(Input::eMouseButton, bool)>&& fn);
        virtual void setMouseAxis(std::function<void(Input::eAxisAxis, float)>&& fn);
        virtual void setReceivesTouch(bool x);
        virtual void setTouchDown(std::function<void(const Input::STouchEvent&)>&& fn);
        virtual void setTouchMotion(std::function<void(const Input::STouchEvent&)>&& fn);
        virtual void setTouchUp(std::function<void(const Input::STouchEvent&)>&& fn);
        virtual void setTouchCancel(std::function<void(const Input::STouchEvent&)>&& fn);

        virtual void setTooltip(std::string&&);

        virtual void setRepositioned(std::function<void()>&& fn);

        virtual void setGrow(bool grow);
        virtual void setGrow(bool growH, bool growV);

        // Installs a persistent policy. Future property/layout changes are animated.
        // Geometry is presentation-only; layout and input immediately use final coordinates.
        void animateOpacity(const SAnimation& animation);
        void animateGeometry(const SAnimation& animation);
        void setOpacity(float opacity);

        // forces a reposition right now, useful for pre-calculating expected sizes
        virtual void forceReposition();

        HT_HIDDEN :

            /* Sizes for auto positioning in layouts */
            virtual std::optional<Abyssutils::Math::Vector2D>
                                                         preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);

        virtual bool                                     acceptsMouseInput();
        virtual bool                                     acceptsKeyboardInput();
        virtual bool                                     acceptsTouchInput();
        virtual ePointerShape                            pointerShape();
        virtual std::function<ePointerShape()>           pointerShapeFn();
        virtual bool                                     alwaysGetMouseInput();
        virtual void                                     imCommitNewText(const std::string& text);
        virtual void                                     imApplyText();

        virtual void                                     recheckColor();
        virtual bool                                     positioningDependsOnChild();
        virtual Abyssutils::Math::CBox                    opaqueBox();

        //

        Abyssutils::Memory::CUniquePointer<SElementInternalData> impl;

      protected:
        IElement();
    };
};
