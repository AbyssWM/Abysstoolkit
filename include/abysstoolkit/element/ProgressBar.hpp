#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssutils/memory/UniquePtr.hpp>

namespace Abysstoolkit {

    struct SProgressBarImpl;
    struct SProgressBarData;
    class CProgressBarElement;

    class CProgressBarBuilder {
      public:
        ~CProgressBarBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CProgressBarBuilder> begin();

        // 0.0 - 1.0 (clamped). ignored when indeterminate is true.
        Abyssutils::Memory::CSharedPointer<CProgressBarBuilder>        value(float);

        // when true, the bar shows a looping pulse instead of value-driven fill
        Abyssutils::Memory::CSharedPointer<CProgressBarBuilder>        indeterminate(bool);

        Abyssutils::Memory::CSharedPointer<CProgressBarBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CProgressBarElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CProgressBarBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SProgressBarData>  m_data;
        Abyssutils::Memory::CWeakPointer<CProgressBarElement> m_element;

        CProgressBarBuilder() = default;

        friend class CProgressBarElement;
    };

    class CProgressBarElement : public IElement {
      public:
        virtual ~CProgressBarElement() = default;

        Abyssutils::Memory::CSharedPointer<CProgressBarBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                      size();

        // imperative updates that skip the builder allocation
        float                                                  value();
        void                                                   setValue(float);

      private:
        CProgressBarElement(const SProgressBarData& data);
        static Abyssutils::Memory::CSharedPointer<CProgressBarElement> create(const SProgressBarData& data);

        void                                                          replaceData(const SProgressBarData& data);

        virtual void                                                  paint();
        virtual void                                                  reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>              preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>              minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>              maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                                  positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SProgressBarImpl>           m_impl;

        friend class CProgressBarBuilder;
    };
};
