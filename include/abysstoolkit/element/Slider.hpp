#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssgraphics/color/Color.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <functional>

namespace Abysstoolkit {

    struct SSliderImpl;
    struct SSliderData;
    class CSliderElement;

    class CSliderBuilder {
      public:
        ~CSliderBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CSliderBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        onChanged(std::function<void(Abyssutils::Memory::CSharedPointer<CSliderElement>, float)>&&);
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        min(float);
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        max(float);
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        val(float);
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        snapInt(bool);
        Abyssutils::Memory::CSharedPointer<CSliderBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CSliderElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CSliderBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SSliderData>  m_data;
        Abyssutils::Memory::CWeakPointer<CSliderElement> m_element;

        CSliderBuilder() = default;

        friend class CSliderElement;
    };

    class CSliderElement : public IElement {
      public:
        virtual ~CSliderElement() = default;

        Abyssutils::Memory::CSharedPointer<CSliderBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                 size();
        bool                                              sliding();

      private:
        CSliderElement(const SSliderData& data);
        static Abyssutils::Memory::CSharedPointer<CSliderElement> create(const SSliderData& data);

        void                                                     replaceData(const SSliderData& data);

        virtual void                                             paint();
        virtual void                                             reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>         preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>         minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>         maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                             acceptsMouseInput();
        virtual ePointerShape                                    pointerShape();
        virtual bool                                             positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SSliderImpl>           m_impl;

        friend class CSliderBuilder;
        friend class CSliderSlider;
    };
};
