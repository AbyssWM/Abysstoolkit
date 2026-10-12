#pragma once

#include <abysstoolkit/element/Spinbox.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/RowLayout.hpp>
#include <abysstoolkit/element/Text.hpp>
#include <abysstoolkit/element/Null.hpp>

#include "../../helpers/Memory.hpp"
#include "../../renderer/Polygon.hpp"
#include "../../core/AnimatedVariable.hpp"

namespace Abysstoolkit {
    class CSpinboxSpinner;

    struct SSpinboxAngleData {
        CDynamicSize        size        = {CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
        colorFn             color       = [] { return CAbyssColor{1.F, 0.F, 0.F}; };
        colorFn             colorActive = [] { return CAbyssColor{1.F, 1.F, 0.F}; };
        bool                right       = false;
        WP<CSpinboxSpinner> spinner;
    };

    class CSpinboxAngleElement : public IElement {
      public:
        static Abyssutils::Memory::CSharedPointer<CSpinboxAngleElement> create(const SSpinboxAngleData& data);
        virtual ~CSpinboxAngleElement() = default;

      private:
        CSpinboxAngleElement(const SSpinboxAngleData& data);

        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual Abyssutils::Math::Vector2D                size();
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                     acceptsMouseInput();
        virtual ePointerShape                            pointerShape();

        SSpinboxAngleData                                m_data;
        CPolygon                                         m_poly;
        bool                                             m_primedForUp = false;

        PHLANIMVAR<CAbyssColor>                           m_color;

        friend class CCheckboxElement;
    };

    struct SSpinboxData {
        std::string                                                                     label       = "Choose one";
        std::vector<std::string>                                                        items       = {"Item A", "Item B"};
        size_t                                                                          currentItem = 0;
        std::function<void(Abyssutils::Memory::CSharedPointer<CSpinboxElement>, size_t)> onChanged;
        CDynamicSize                                                                    size{CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {}};
        bool                                                                            fill = false;
    };

    class CSpinboxSpinner : public IElement {
      public:
        static Abyssutils::Memory::CSharedPointer<CSpinboxSpinner> create(SP<CSpinboxElement> element);
        virtual ~CSpinboxSpinner() = default;

        void updateLabel(const std::string& str);

      private:
        CSpinboxSpinner(SP<CSpinboxElement> element);

        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual Abyssutils::Math::Vector2D                size();
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                     acceptsMouseInput();
        virtual ePointerShape                            pointerShape();
        virtual bool                                     alwaysGetMouseInput();

        void                                             moveSelection(bool forward);
        void                                             init();

        SP<CSpinboxElement>                              m_parent;

        SP<CRectangleElement>                            m_background;
        SP<CRowLayoutElement>                            m_layout;
        SP<CTextElement>                                 m_label;
        SP<CSpinboxAngleElement>                         m_left;
        SP<CSpinboxAngleElement>                         m_right;
        SP<CNullElement>                                 m_leftPad, m_rightPad;

        WP<CSpinboxSpinner>                              m_self;

        friend class CSpinboxElement;
        friend class CSpinboxAngleElement;
    };

    struct SSpinboxImpl {
        SSpinboxData          data;

        WP<CSpinboxElement>   self;

        SP<CRowLayoutElement> layout;
        SP<CTextElement>      label;
        SP<CNullElement>      spacer;
        SP<CSpinboxSpinner>   spinner;
    };
}
