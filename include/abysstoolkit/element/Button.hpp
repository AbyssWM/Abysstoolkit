#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../types/FontTypes.hpp"

#include <abyssgraphics/color/Color.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <functional>

namespace Abysstoolkit {

    struct SButtonImpl;
    class CButtonElement;
    struct SButtonData;

    class CButtonBuilder {
      public:
        ~CButtonBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CButtonBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        label(std::string&&);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        noBorder(bool);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        noBg(bool);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        alignText(eFontAlignment);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        fontFamily(std::string&&);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        fontSize(CFontSize&&);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        onMainClick(std::function<void(Abyssutils::Memory::CSharedPointer<CButtonElement>)>&&);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        onRightClick(std::function<void(Abyssutils::Memory::CSharedPointer<CButtonElement>)>&&);
        Abyssutils::Memory::CSharedPointer<CButtonBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CButtonElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CButtonBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SButtonData>  m_data;
        Abyssutils::Memory::CWeakPointer<CButtonElement> m_element;

        CButtonBuilder() = default;

        friend class CButtonElement;
    };

    class CButtonElement : public IElement {
      public:
        virtual ~CButtonElement() = default;

        Abyssutils::Memory::CSharedPointer<CButtonBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                 size();

      private:
        CButtonElement(const SButtonData& data);
        static Abyssutils::Memory::CSharedPointer<CButtonElement> create(const SButtonData& data);

        void                                                     replaceData(const SButtonData& data);

        virtual void                                             paint();
        virtual void                                             reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>         preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>         minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>         maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                             acceptsMouseInput();
        virtual ePointerShape                                    pointerShape();
        virtual bool                                             positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SButtonImpl>           m_impl;

        friend class CButtonBuilder;
    };
};
