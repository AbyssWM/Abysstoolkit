#pragma once

#include <cstddef>

#include <abysstoolkit/types/ImageTypes.hpp>
#include <abyssutils/math/Vector2D.hpp>

namespace Abysstoolkit {
    class IRendererTexture {
      public:
        IRendererTexture()          = default;
        virtual ~IRendererTexture() = default;

        enum eTextureType : uint8_t {
            TEXTURE_GL,
        };

        virtual size_t                    id()      = 0;
        virtual eTextureType              type()    = 0;
        virtual void                      destroy() = 0;
        virtual eImageFitMode             fitMode() = 0;
        virtual Abyssutils::Math::Vector2D size()    = 0;
    };
}
