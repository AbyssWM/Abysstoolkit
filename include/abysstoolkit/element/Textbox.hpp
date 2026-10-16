#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"

#include <abyssutils/memory/UniquePtr.hpp>
#include <tuple>

namespace Abysstoolkit {

    struct STextboxImpl;
    struct STextboxData;
    class CTextboxElement;

    class CTextboxBuilder {
      public:
        ~CTextboxBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CTextboxBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        placeholder(std::string&&);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        defaultText(std::string&&);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        onTextEdited(std::function<void(Abyssutils::Memory::CSharedPointer<CTextboxElement>, const std::string&)>&&);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        multiline(bool);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        password(bool);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        eyeIcon(bool);
        Abyssutils::Memory::CSharedPointer<CTextboxBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CTextboxElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CTextboxBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<STextboxData>  m_data;
        Abyssutils::Memory::CWeakPointer<CTextboxElement> m_element;

        CTextboxBuilder() = default;

        friend class CTextboxElement;
    };

    class CTextboxElement : public IElement {
      public:
        virtual ~CTextboxElement() = default;

        Abyssutils::Memory::CSharedPointer<CTextboxBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                  size();
        void                                               focus(bool focus = true);
        std::string_view                                   currentText();
        size_t                                             cursorPos() const;
        std::tuple<ssize_t, ssize_t>                       selection() const;
        void                                               setText(std::string text);
        void                                               setPassword(bool password);

      private:
        static Abyssutils::Memory::CSharedPointer<CTextboxElement> create(const STextboxData& data);
        CTextboxElement(const STextboxData& data);

        void                                             replaceData(const STextboxData& data);
        void                                             init();

        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent, bool grow = true);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                     acceptsMouseInput();
        virtual ePointerShape                            pointerShape();
        virtual std::function<ePointerShape()>           pointerShapeFn();
        virtual bool                                     acceptsKeyboardInput();
        virtual void                                     imCommitNewText(const std::string&);
        virtual void                                     imApplyText();
        virtual bool                                     positioningDependsOnChild();

        Abyssutils::Memory::CUniquePointer<STextboxImpl>  m_impl;

        friend class CTextboxBuilder;
    };
};
