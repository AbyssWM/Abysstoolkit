#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../palette/Color.hpp"
#include "../types/FontTypes.hpp"

#include <abyssgraphics/resource/resources/TextResource.hpp>

#include <abyssutils/memory/Atomic.hpp>

#include <optional>

namespace Abysstoolkit {

    class IRendererTexture;
    struct STextImpl;
    struct STextData;
    class CTextElement;

    class CTextBuilder {
      public:
        ~CTextBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CTextBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        color(colorFn&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        a(float);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        fontSize(CFontSize&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        align(eFontAlignment);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        text(std::string&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        fontFamily(std::string&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        clampSize(Abyssutils::Math::Vector2D&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        callback(std::function<void()>&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        noEllipsize(bool);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        size(CDynamicSize&&);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        async(bool x);
        Abyssutils::Memory::CSharedPointer<CTextBuilder>        interactable(bool x);

        Abyssutils::Memory::CSharedPointer<CTextElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CTextBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<STextData>  m_data;
        Abyssutils::Memory::CWeakPointer<CTextElement> m_element;

        CTextBuilder() = default;

        friend class CTextElement;
    };

    class CTextElement : public IElement {
      public:
        virtual ~CTextElement();

        Abyssutils::Memory::CSharedPointer<CTextBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D               size();

        HT_HIDDEN : CTextElement(const STextData& data);
        static Abyssutils::Memory::CSharedPointer<CTextElement> create(const STextData& data);

        void                                                   replaceData(const STextData& data);

        virtual void                                           paint();
        virtual void                                           reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>       preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>       minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>       maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                           positioningDependsOnChild();
        virtual void                                           recheckColor();
        virtual bool                                           acceptsMouseInput();
        virtual std::function<ePointerShape()>                 pointerShapeFn();

        Abyssutils::Memory::CUniquePointer<STextImpl>           m_impl;

        friend class CButtonElement;
        friend class CTextBuilder;
        friend struct STextboxImpl;
        friend class CTextboxElement;
    };
};
