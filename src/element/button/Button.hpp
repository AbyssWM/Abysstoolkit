#pragma once

#include <abysstoolkit/element/Button.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/Text.hpp>

#include "../../helpers/Memory.hpp"
#include "../../core/InternalBackend.hpp"

namespace Abysstoolkit {
    struct SButtonData {
        std::string                                                            label      = "Click me";
        bool                                                                   noBorder   = false;
        bool                                                                   noBg       = false;
        std::string                                                            fontFamily = g_palette ? g_palette->m_vars.fontFamily : "Sans Serif";
        CFontSize                                                              fontSize   = {CFontSize::HT_FONT_TEXT};
        eFontAlignment                                                         alignText  = HT_FONT_ALIGN_CENTER;
        std::function<void(Abyssutils::Memory::CSharedPointer<CButtonElement>)> onMainClick;
        std::function<void(Abyssutils::Memory::CSharedPointer<CButtonElement>)> onRightClick;
        CDynamicSize                                                           size{CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {1, 1}};
    };

    struct SButtonImpl {
        SButtonData           data;

        WP<CButtonElement>    self;
        SP<CRectangleElement> background;
        SP<CTextElement>      label;

        bool                  labelChanged = true;
    };
}
