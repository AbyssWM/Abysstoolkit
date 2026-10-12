
#pragma once

#include <abysstoolkit/element/Text.hpp>
#include <pango/pangocairo.h>

#include <mutex>
#include <atomic>

#include "../../helpers/Memory.hpp"
#include "../../core/InternalBackend.hpp"

namespace Abysstoolkit {
    struct STextData {
        std::string                              text;
        std::string                              fontFamily = g_palette ? g_palette->m_vars.fontFamily : "Sans Serif";
        CFontSize                                fontSize{CFontSize::HT_FONT_TEXT};
        eFontAlignment                           align       = HT_FONT_ALIGN_LEFT;
        colorFn                                  color       = [] { return g_backend->getPalette()->m_colors.text; };
        float                                    a           = 1.F;
        bool                                     noEllipsize = false;
        std::optional<Abyssutils::Math::Vector2D> clampSize;
        CDynamicSize                             size{CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
        std::function<void()>                    callback; // called after resource is loaded
        bool                                     async = true;
        std::optional<bool>                      interactable;
    };

    struct STextLink {
        uint64_t                 begin = 0, end = 0;
        std::string              link;
        Abyssutils::Math::CRegion region;
    };

    struct STextImpl {
        STextData                                                                                      data;

        std::string                                                                                    parsedText;
        std::vector<STextLink>                                                                         parsedLinks;
        STextLink*                                                                                     hoveredTextLink = nullptr;

        WP<CTextElement>                                                                               self;

        size_t                                                                                         lastFontSizeUnscaled = 0;
        float                                                                                          lastScale            = 1.F;
        bool                                                                                           needsTexRefresh = false, newTex = false;

        Abyssutils::Math::Vector2D                                                                      lastMaxSize;

        SP<IRendererTexture>                                                                           tex;
        SP<IRendererTexture>                                                                           oldTex; // while loading a new one
        ASP<Abyssgraphics::CTextResource>                                                               resource;
        Abyssutils::Math::Vector2D                                                                      size, preferred;

        Abyssutils::Math::Vector2D                                                                      lastCursorPos;

        bool                                                                                           waitingForTex = false;

        Abyssutils::Math::Vector2D                                                                      getTextSizePreferred();
        Abyssutils::Math::CBox                                                                          getCharBox(size_t offset);
        std::optional<size_t>                                                                          vecToOffset(const Abyssutils::Math::Vector2D& vec);
        float                                                                                          getCursorPos(size_t offset);
        float                                                                                          getCursorPos(const Abyssutils::Math::Vector2D& click);
        Abyssutils::Math::Vector2D                                                                      unscale(const Abyssutils::Math::Vector2D& x);
        std::tuple<UP<Abyssgraphics::CCairoSurface>, cairo_t*, PangoLayout*, Abyssutils::Math::Vector2D> prepPangoLayout();
        void                                                                                           scheduleTexRefresh();
        void                                                                                           renderTex();
        void                                                                                           postTexLoad();
        void                                                                                           parseText();
        void                                                                                           recheckTextBoxes();
        void                                                                                           onMouseDown();
        void                                                                                           onMouseMove();

        friend class CTextboxElement;
        friend struct STextboxImpl;
    };
}
