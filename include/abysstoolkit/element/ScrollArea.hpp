#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

namespace Abysstoolkit {
    struct SScrollAreaData;
    struct SScrollAreaImpl;
    class CScrollAreaElement;

    class CScrollAreaBuilder {
      public:
        ~CScrollAreaBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder>        scrollX(bool);
        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder>        scrollY(bool);
        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder>        blockUserScroll(bool);
        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder>        showScrollbar(bool);
        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CScrollAreaElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CScrollAreaBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SScrollAreaData>  m_data;
        Abyssutils::Memory::CWeakPointer<CScrollAreaElement> m_element;

        CScrollAreaBuilder() = default;

        friend class CScrollAreaElement;
    };

    class CScrollAreaElement : public IElement {
      public:
        virtual ~CScrollAreaElement() = default;

        Abyssutils::Memory::CSharedPointer<CScrollAreaBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                     size();

        Abyssutils::Math::Vector2D                             getCurrentScroll();
        void                                                  setScroll(const Abyssutils::Math::Vector2D&);

        // user children are routed into the inner scrolled layer
        virtual void addChild(Abyssutils::Memory::CSharedPointer<IElement> child);
        virtual void removeChild(Abyssutils::Memory::CSharedPointer<IElement> child);
        virtual void clearChildren();

      private:
        CScrollAreaElement(const SScrollAreaData& data);
        static Abyssutils::Memory::CSharedPointer<CScrollAreaElement> create(const SScrollAreaData& data);

        void                                                         init();
        void                                                         replaceData(const SScrollAreaData& data);

        // builds or tears down the scrollbar elements to match showScrollbar
        void rebuildScrollbars();
        // sizes, places and shows/hides the scrollbars for the current scroll state
        void                                                         layoutScrollbars();

        virtual void                                                 paint();
        virtual void                                                 reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>             preferredSize(const Abyssutils::Math::Vector2D& parent, bool grow = true);
        virtual std::optional<Abyssutils::Math::Vector2D>             minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                                 acceptsMouseInput();
        virtual bool                                                 alwaysGetMouseInput();
        virtual ePointerShape                                        pointerShape();
        virtual bool                                                 positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SScrollAreaImpl>           m_impl;

        friend class CScrollAreaBuilder;
    };
};
