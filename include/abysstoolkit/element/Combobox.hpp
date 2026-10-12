#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssgraphics/color/Color.hpp>
#include <abyssutils/memory/UniquePtr.hpp>

#include <functional>
#include <vector>

namespace Abysstoolkit {

    struct SComboboxImpl;
    struct SComboboxData;
    class CComboboxElement;

    class CComboboxBuilder {
      public:
        ~CComboboxBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CComboboxBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CComboboxBuilder>        items(std::vector<std::string>&&);
        Abyssutils::Memory::CSharedPointer<CComboboxBuilder>        currentItem(size_t);
        Abyssutils::Memory::CSharedPointer<CComboboxBuilder>        onChanged(std::function<void(Abyssutils::Memory::CSharedPointer<CComboboxElement>, size_t)>&&);
        Abyssutils::Memory::CSharedPointer<CComboboxBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CComboboxElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CComboboxBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SComboboxData>  m_data;
        Abyssutils::Memory::CWeakPointer<CComboboxElement> m_element;

        CComboboxBuilder() = default;

        friend class CComboboxElement;
    };

    class CComboboxElement : public IElement {
      public:
        virtual ~CComboboxElement() = default;

        Abyssutils::Memory::CSharedPointer<CComboboxBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                   size();
        size_t                                              current();
        void                                                setCurrent(size_t current);

      private:
        CComboboxElement(const SComboboxData& data);
        static Abyssutils::Memory::CSharedPointer<CComboboxElement> create(const SComboboxData& data);

        void                                                       replaceData(const SComboboxData& data);

        virtual void                                               paint();
        virtual void                                               reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D>           preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>           minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D>           maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                               acceptsMouseInput();
        virtual ePointerShape                                      pointerShape();
        virtual bool                                               positioningDependsOnChild();

        void                                                       setSelection(size_t idx);
        void                                                       init();
        void                                                       updateLabel(const std::string& str);

        void                                                       openDropdown();
        void                                                       closeDropdown();

        Abyssutils::Memory::CUniquePointer<SComboboxImpl>           m_impl;

        friend class CComboboxBuilder;
        friend class CComboboxClickable;
    };
};
