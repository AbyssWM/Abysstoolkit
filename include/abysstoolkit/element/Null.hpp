#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../palette/Color.hpp"

#include <abyssutils/memory/UniquePtr.hpp>

namespace Abysstoolkit {

    struct SNullData;
    struct SNullImpl;
    class CNullElement;

    class CNullBuilder {
      public:
        ~CNullBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CNullBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CNullBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CNullElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CNullBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SNullData>  m_data;
        Abyssutils::Memory::CWeakPointer<CNullElement> m_element;

        CNullBuilder() = default;

        friend class CNullElement;
    };

    class CNullElement : public IElement {
      public:
        virtual ~CNullElement() = default;

        Abyssutils::Memory::CSharedPointer<CNullBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D               size();

      private:
        CNullElement(const SNullData& data);
        static Abyssutils::Memory::CSharedPointer<CNullElement> create(const SNullData& data);

        void                                                   replaceData(const SNullData& data);

        virtual void                                           paint();
        virtual void                                           reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>       preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>       minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>       maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                           positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SNullImpl>           m_impl;

        friend class CNullBuilder;
    };
};
