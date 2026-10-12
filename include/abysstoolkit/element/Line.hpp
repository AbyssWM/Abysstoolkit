#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../palette/Color.hpp"

#include <abyssutils/memory/UniquePtr.hpp>

namespace Abysstoolkit {

    struct SLineImpl;
    struct SLineData;
    class CLineElement;

    class CLineBuilder {
      public:
        ~CLineBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CLineBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CLineBuilder>        color(colorFn&&);
        Abyssutils::Memory::CSharedPointer<CLineBuilder>        thick(int);
        Abyssutils::Memory::CSharedPointer<CLineBuilder>        points(std::vector<Abyssutils::Math::Vector2D>&&); // [0, 0] - [1, 1]
        Abyssutils::Memory::CSharedPointer<CLineBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CLineElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CLineBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SLineData>  m_data;
        Abyssutils::Memory::CWeakPointer<CLineElement> m_element;

        CLineBuilder() = default;

        friend class CLineElement;
    };

    class CLineElement : public IElement {
      public:
        virtual ~CLineElement() = default;

        Abyssutils::Memory::CSharedPointer<CLineBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D               size();

      private:
        static Abyssutils::Memory::CSharedPointer<CLineElement> create(const SLineData& data);
        CLineElement(const SLineData& data);

        void                                             replaceData(const SLineData& data);

        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                     positioningDependsOnChild();
        virtual void                                     recheckColor();

        Abyssutils::Memory::CUniquePointer<SLineImpl>     m_impl;

        friend class CLineBuilder;
    };
};
