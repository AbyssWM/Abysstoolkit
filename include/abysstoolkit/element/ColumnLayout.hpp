#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

namespace Abysstoolkit {

    struct SColumnLayoutData;
    struct SColumnLayoutImpl;
    class CColumnLayoutElement;

    class CColumnLayoutBuilder {
      public:
        ~CColumnLayoutBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CColumnLayoutBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CColumnLayoutBuilder>        gap(size_t gap);
        Abyssutils::Memory::CSharedPointer<CColumnLayoutBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CColumnLayoutElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CColumnLayoutBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SColumnLayoutData>  m_data;
        Abyssutils::Memory::CWeakPointer<CColumnLayoutElement> m_element;

        CColumnLayoutBuilder() = default;

        friend class CColumnLayoutElement;
    };

    class CColumnLayoutElement : public IElement {
      public:
        virtual ~CColumnLayoutElement() = default;

        Abyssutils::Memory::CSharedPointer<CColumnLayoutBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                       size();

      private:
        CColumnLayoutElement(const SColumnLayoutData& data);
        static Abyssutils::Memory::CSharedPointer<CColumnLayoutElement> create(const SColumnLayoutData& data);

        void                                                           replaceData(const SColumnLayoutData& data);

        virtual void                                                   paint();
        virtual void                                                   reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>               preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>               minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                                   positioningDependsOnChild();

        Abyssutils::Math::Vector2D                                      childSize(Abyssutils::Memory::CSharedPointer<IElement> child);

        Abyssutils::Memory::CUniquePointer<SColumnLayoutImpl>           m_impl;

        friend class CColumnLayoutBuilder;
    };
};
