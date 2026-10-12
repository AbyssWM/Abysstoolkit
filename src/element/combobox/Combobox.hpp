#pragma once

#include <abysstoolkit/element/Combobox.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/RowLayout.hpp>
#include <abysstoolkit/element/Text.hpp>
#include <abysstoolkit/element/Null.hpp>

#include "../../helpers/Memory.hpp"
#include "../../helpers/Signal.hpp"
#include "../../renderer/Polygon.hpp"
#include "../../core/AnimatedVariable.hpp"

namespace Abysstoolkit {
    class CComboboxClickable;
    class CButtonElement;
    class CColumnLayoutElement;
    class CScrollAreaElement;
    class IWindow;

    struct SDropdownHandleData {
        CDynamicSize size  = {CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
        colorFn      color = [] { return CAbyssColor{1.F, 0.F, 0.F}; };
    };

    class CDropdownHandleElement : public IElement {
      public:
        static Abyssutils::Memory::CSharedPointer<CDropdownHandleElement> create(const SDropdownHandleData& data);
        virtual ~CDropdownHandleElement() = default;

      private:
        CDropdownHandleElement(const SDropdownHandleData& data);

        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual Abyssutils::Math::Vector2D                size();
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);

        SDropdownHandleData                              m_data;
        CPolygon                                         m_poly;

        friend class CCheckboxElement;
    };

    struct SComboboxData {
        std::vector<std::string>                                                         items       = {"Item A", "Item B"};
        size_t                                                                           currentItem = 0;
        std::function<void(Abyssutils::Memory::CSharedPointer<CComboboxElement>, size_t)> onChanged;
        CDynamicSize                                                                     size{CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {}};
    };

    struct SComboboxImpl {
        SComboboxData              data;

        WP<CComboboxElement>       self;

        SP<CRectangleElement>      background;
        SP<CRowLayoutElement>      layout;
        SP<CTextElement>           label;
        SP<CDropdownHandleElement> handle;
        SP<CNullElement>           leftPad, rightPad, middlePad;

        struct {
            SP<IWindow>                     popup;
            SP<CRectangleElement>           background;
            SP<CColumnLayoutElement>        layout;
            SP<CScrollAreaElement>          scroll;
            std::vector<SP<CButtonElement>> buttons;
        } dropdown;

        bool primedForUp = false;

        struct {
            CAbyssSignalListener clicked;
            CAbyssSignalListener popupClosed;
        } listeners;
    };
}
