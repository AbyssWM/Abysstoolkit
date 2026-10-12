#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../palette/Color.hpp"

#include <abyssutils/memory/UniquePtr.hpp>

namespace Abysstoolkit {

    struct SRectangleImpl;
    struct SRectangleData;
    class CRectangleElement;

    class CRectangleBuilder {
      public:
        ~CRectangleBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CRectangleBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CRectangleBuilder>        color(colorFn&&);
        Abyssutils::Memory::CSharedPointer<CRectangleBuilder>        borderColor(colorFn&&);
        Abyssutils::Memory::CSharedPointer<CRectangleBuilder>        rounding(int);
        Abyssutils::Memory::CSharedPointer<CRectangleBuilder>        borderThickness(int);
        Abyssutils::Memory::CSharedPointer<CRectangleBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CRectangleElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CRectangleBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SRectangleData>  m_data;
        Abyssutils::Memory::CWeakPointer<CRectangleElement> m_element;

        CRectangleBuilder() = default;

        friend class CRectangleElement;
    };

    class CRectangleElement : public IElement {
      public:
        virtual ~CRectangleElement() = default;

        Abyssutils::Memory::CSharedPointer<CRectangleBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                    size();

      private:
        static Abyssutils::Memory::CSharedPointer<CRectangleElement> create(const SRectangleData& data);
        CRectangleElement(const SRectangleData& data);

        void                                              replaceData(const SRectangleData& data);

        virtual void                                      paint();
        virtual void                                      reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>  preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>  minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>  maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                      positioningDependsOnChild();
        virtual void                                      recheckColor();
        virtual Abyssutils::Math::CBox                     opaqueBox();

        Abyssutils::Memory::CUniquePointer<SRectangleImpl> m_impl;

        friend class CRectangleBuilder;
    };
};
