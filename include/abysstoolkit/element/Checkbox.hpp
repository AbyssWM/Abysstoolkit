#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssgraphics/color/Color.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <functional>

namespace Abysstoolkit {

    struct SCheckboxImpl;
    struct SCheckboxData;
    class CCheckboxElement;

    class CCheckboxBuilder {
      public:
        ~CCheckboxBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CCheckboxBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CCheckboxBuilder>        onToggled(std::function<void(Abyssutils::Memory::CSharedPointer<CCheckboxElement>, bool)>&&);
        Abyssutils::Memory::CSharedPointer<CCheckboxBuilder>        toggled(bool);
        Abyssutils::Memory::CSharedPointer<CCheckboxBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CCheckboxElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CCheckboxBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SCheckboxData>  m_data;
        Abyssutils::Memory::CWeakPointer<CCheckboxElement> m_element;

        CCheckboxBuilder() = default;

        friend class CCheckboxElement;
    };

    class CCheckboxElement : public IElement {
      public:
        virtual ~CCheckboxElement() = default;

        Abyssutils::Memory::CSharedPointer<CCheckboxBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                   size();
        bool                                                state();
        void                                                setState(bool state);

      private:
        CCheckboxElement(const SCheckboxData& data);
        static Abyssutils::Memory::CSharedPointer<CCheckboxElement> create(const SCheckboxData& data);

        void                                                       replaceData(const SCheckboxData& data);

        virtual void                                               paint();
        virtual void                                               reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>           preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>           minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>           maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                               acceptsMouseInput();
        virtual ePointerShape                                      pointerShape();
        virtual bool                                               positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SCheckboxImpl>           m_impl;

        friend class CCheckboxBuilder;
    };
};
