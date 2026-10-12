#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssgraphics/color/Color.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <functional>
#include <vector>

namespace Abysstoolkit {

    struct SSpinboxImpl;
    struct SSpinboxData;
    class CSpinboxElement;

    class CSpinboxBuilder {
      public:
        ~CSpinboxBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CSpinboxBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        label(std::string&&);
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        items(std::vector<std::string>&&);
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        currentItem(size_t);
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        onChanged(std::function<void(Abyssutils::Memory::CSharedPointer<CSpinboxElement>, size_t)>&&);
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        fill(bool);
        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CSpinboxElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CSpinboxBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SSpinboxData>  m_data;
        Abyssutils::Memory::CWeakPointer<CSpinboxElement> m_element;

        CSpinboxBuilder() = default;

        friend class CSpinboxElement;
    };

    class CSpinboxElement : public IElement {
      public:
        virtual ~CSpinboxElement() = default;

        Abyssutils::Memory::CSharedPointer<CSpinboxBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                  size();
        size_t                                             current();
        void                                               setCurrent(size_t current);

      private:
        CSpinboxElement(const SSpinboxData& data);
        static Abyssutils::Memory::CSharedPointer<CSpinboxElement> create(const SSpinboxData& data);

        void                                                      replaceData(const SSpinboxData& data);

        void                                                      init();

        virtual void                                              paint();
        virtual void                                              reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>          preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>          minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>          maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                              acceptsMouseInput();
        virtual ePointerShape                                     pointerShape();
        virtual bool                                              positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<SSpinboxImpl>           m_impl;

        friend class CSpinboxSpinner;
        friend class CSpinboxBuilder;
    };
};
