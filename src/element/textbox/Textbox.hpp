#pragma once

#include <abysstoolkit/element/Textbox.hpp>
#include <abyssutils/signal/Listener.hpp>

#include "../../helpers/Memory.hpp"

using namespace Abyssutils::Signal;

namespace Abysstoolkit {

    class CRectangleElement;
    class CNullElement;
    class CTextElement;

    struct STextboxData {
        std::string                                                                                 text;
        std::string                                                                                 placeholder;
        std::function<void(Abyssutils::Memory::CSharedPointer<CTextboxElement>, const std::string&)> onTextEdited;
        bool                                                                                        multiline = true;
        bool                                                                                        password  = false;
        bool                                                                                        eyeIcon   = false;
        CDynamicSize                                                                                size{CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
    };
    struct STextboxImpl {
        STextboxData                       data;

        WP<CTextboxElement>                self;

        SP<CRectangleElement>              bg;
        SP<CNullElement>                   bgInnerCont;
        SP<CNullElement>                   cursorCont;
        SP<CRectangleElement>              cursor;
        SP<CNullElement>                   selectBgCont;
        std::vector<SP<CRectangleElement>> selectBgs;
        SP<CTextElement>                   text;
        SP<CTextElement>                   placeholder;
        SP<CRectangleElement>              eyeBg;
        SP<CTextElement>                   eyeText;

        static constexpr float             EYE_W = 28.F;

        bool                               active            = false;
        bool                               firstAttachedPass = false;
        bool                               eyeHover          = false;

        struct {
            CAbyssSignalListener key;
            CAbyssSignalListener enter;
            CAbyssSignalListener leave;
            CAbyssSignalListener mouseMove;
            CAbyssSignalListener mouseButton;
            CAbyssSignalListener mouseLeave;
        } listeners;

        struct {
            size_t      cursor = 0;
            std::string imText;
            ssize_t     selectBegin = -1, selectEnd = -1;
        } inputState;

        void                      clearSelect();
        void                      updateSelect();
        void                      updateEyeIcon();
        void                      updateEyeSymbol();
        bool                      hasSelect() const;
        void                      removeSelectedText();
        void                      focusCursorAtClickedChar();
        size_t                    moveLineBackwards() const;
        size_t                    moveLineForwards() const;
        size_t                    moveWordBackwards() const;
        size_t                    moveWordForwards() const;
        size_t                    moveCharBackwards() const;
        size_t                    moveCharForwards() const;
        void                      updateLabel(bool textEdited = false);
        void                      updateCursor();
        size_t                    srcToDisplay(size_t srcByte) const;
        size_t                    displayToSrc(size_t displayByte) const;

        Abyssutils::Math::Vector2D lastCursorPos;
    };
}
