#include <abysstoolkit/element/Image.hpp>

#include "../../helpers/Memory.hpp"
#include "../../resource/assetCache/AssetCacheEntry.hpp"

namespace Abysstoolkit {
    struct SImageData {
        std::string                path;
        float                      a        = 1.F;
        int                        rounding = 0;
        bool                       sync     = false;
        SP<ISystemIconDescription> icon;
        std::vector<uint8_t>       data;
        eImageFitMode              fitMode = IMAGE_FIT_MODE_STRETCH;
        CDynamicSize               size{CDynamicSize::HT_SIZE_PERCENT, CDynamicSize::HT_SIZE_PERCENT, {1, 1}};
    };

    struct SImageImpl {
        SImageData                                                            data;

        WP<CImageElement>                                                     self;

        float                                                                 lastScale = 1.F;

        Abyssutils::Memory::CSharedPointer<Asset::CAssetCacheEntry>            cacheEntry;
        Abyssutils::Memory::CSharedPointer<Asset::CAssetCacheEntry>            oldCacheEntry; // while loading a new one
        Abyssutils::Memory::CAtomicSharedPointer<Abyssgraphics::CImageResource> resource;
        Abyssutils::Math::Vector2D                                             size;

        bool                                                                  waitingForTex = false, failed = false;

        std::string                                                           lastPath = "";
        void*                                                                 lastData = nullptr;

        Abyssutils::Math::Vector2D                                             preferredSvgSize();
        void                                                                  postImageLoad();
        void                                                                  postImageScheduleRecalc();
        std::string                                                           getCacheString();

        struct {
            Abyssutils::Signal::CAbyssSignalListener cacheEntryDone;
        } listeners;
    };
}
