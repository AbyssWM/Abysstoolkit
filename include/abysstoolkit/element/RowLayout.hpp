#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

namespace Abysstoolkit {
    struct SRowLayoutData;
    struct SRowLayoutImpl;
    class CRowLayoutElement;

    class CRowLayoutBuilder {
      public:
        ~CRowLayoutBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CRowLayoutBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CRowLayoutBuilder>        gap(size_t gap);
        Abyssutils::Memory::CSharedPointer<CRowLayoutBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CRowLayoutElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CRowLayoutBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SRowLayoutData>  m_data;
        Abyssutils::Memory::CWeakPointer<CRowLayoutElement> m_element;

        CRowLayoutBuilder() = default;

        friend class CRowLayoutElement;
    };

    class CRowLayoutElement : public IElement {
      public:
        virtual ~CRowLayoutElement() = default;

        Abyssutils::Memory::CSharedPointer<CRowLayoutBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                    size();

      private:
        CRowLayoutElement(const SRowLayoutData& data);
        static Abyssutils::Memory::CSharedPointer<CRowLayoutElement> create(const SRowLayoutData& data);

        void                                                        replaceData(const SRowLayoutData& data);

        virtual void                                                paint();
        virtual void                                                reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>            preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>            minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                                positioningDependsOnChild();
        Abyssutils::Math::Vector2D                                   childSize(Abyssutils::Memory::CSharedPointer<IElement> child);

        Abyssutils::Memory::CUniquePointer<SRowLayoutImpl>           m_impl;

        friend class CCheckboxElement;
        friend class CSpinboxElement;
        friend class CSpinboxSpinner;
        friend class CRowLayoutBuilder;
        friend class CSliderSlider;
        friend class CSliderElement;
        friend class CComboboxElement;
        friend class CComboboxClickable;
    };
};
