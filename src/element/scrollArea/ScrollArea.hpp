#include <abysstoolkit/element/ScrollArea.hpp>

#include "../../helpers/Memory.hpp"
#include "../../helpers/Signal.hpp"

namespace Abysstoolkit {
    struct SScrollAreaData {
        CDynamicSize              size            = CDynamicSize(CDynamicSize::HT_SIZE_AUTO, CDynamicSize::HT_SIZE_AUTO, {1, 1});
        bool                      scrollX         = false;
        bool                      scrollY         = false;
        bool                      blockUserScroll = false;
        Abyssutils::Math::Vector2D currentScroll;
    };

    struct SScrollAreaImpl {
        SScrollAreaData        data;

        WP<CScrollAreaElement> self;

        void                   clampMaxScroll();

        struct {
            CAbyssSignalListener axis;
        } listeners;
    };
}
