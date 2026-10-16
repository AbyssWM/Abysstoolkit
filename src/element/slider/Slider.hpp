#pragma once

#include <abysstoolkit/element/Slider.hpp>
#include <abysstoolkit/element/Rectangle.hpp>
#include <abysstoolkit/element/RowLayout.hpp>
#include <abysstoolkit/element/Text.hpp>
#include <abysstoolkit/element/Null.hpp>

#include "../../helpers/Memory.hpp"

namespace Abysstoolkit {

    struct SSliderData {
        float                                                                         min = 0, max = 100, current = 50;
        bool                                                                          snapInt = true;
        std::function<void(Abyssutils::Memory::CSharedPointer<CSliderElement>, float)> onChanged;
        CDynamicSize                                                                  size{CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {}};
    };

    struct SSliderImpl {
        SSliderData               data;

        WP<CSliderElement>        self;

        void                      updateValue();

        std::string               valueAsText();
        void                      setPercentage(float percentage);
        void                      refreshValue();
        void                      normalizeValue();
        float                     percentage() const;
        float                     maxLabelSize();

        SP<CRowLayoutElement>     layout;
        SP<CRectangleElement>     background;
        SP<CRectangleElement>     foreground;
        SP<CTextElement>          valueText;
        SP<CNullElement>          textContainer;

        bool                      dragging = false;
        Abyssutils::Math::Vector2D lastPosLocal;

        bool                      labelChanged = true;
    };
}
