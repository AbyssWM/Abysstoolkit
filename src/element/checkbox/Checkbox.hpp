#pragma once

#include <abysstoolkit/element/Checkbox.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/RowLayout.hpp>
#include <abysstoolkit/element/Text.hpp>
#include <abysstoolkit/element/Null.hpp>

#include "../../helpers/Memory.hpp"
#include "../../core/AnimatedVariable.hpp"

namespace Abysstoolkit {
    struct SCheckmarkData {
        CDynamicSize size  = {CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
        colorFn      color = [] { return CAbyssColor{1.F, 0.F, 0.F}; };
    };

    class CCheckmarkElement : public IElement {
      public:
        static Abyssutils::Memory::CSharedPointer<CCheckmarkElement> create(const SCheckmarkData& data);
        virtual ~CCheckmarkElement() = default;

      private:
        CCheckmarkElement(const SCheckmarkData& data);

        virtual void                                     paint();
        virtual void                                     recheckColor();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual Abyssutils::Math::Vector2D                size();
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);

        SCheckmarkData                                   m_data;

        PHLANIMVAR<CAbyssColor>                           m_color;

        friend class CCheckboxElement;
    };

    struct SCheckboxData {
        bool                                                                           toggled = false;
        eCheckboxStyle                                                                 style   = HT_CHECKBOX_STYLE_CHECKMARK;
        std::function<void(Abyssutils::Memory::CSharedPointer<CCheckboxElement>, bool)> onToggled;
        CDynamicSize                                                                   size{CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {}};
    };

    struct SCheckboxImpl {
        SCheckboxData                                   data;

        std::function<void(SP<CCheckboxElement>, bool)> onToggledInternal;

        WP<CCheckboxElement>                            self;
        SP<CRectangleElement>                           background;
        SP<IElement>                                    foreground;

        bool                                            labelChanged = true;

        bool                                            primedForUp = false;
    };
}
