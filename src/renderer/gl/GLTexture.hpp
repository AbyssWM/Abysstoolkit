#pragma once

#include <abyssutils/math/Vector2D.hpp>
#include <abyssgraphics/resource/resources/AsyncResource.hpp>

#include "GL.hpp"

#include <cstdint>

#include "../RendererTexture.hpp"
#include "../../helpers/Memory.hpp"

namespace Abysstoolkit {
    struct SBackendLifetime;
    enum eGLTextureType : uint8_t {
        TEXTURE_INVALID,  // Invalid
        TEXTURE_RGBA,     // 4 channels
        TEXTURE_RGBX,     // discard A
        TEXTURE_EXTERNAL, // EGLImage
    };

    class CTimer;

    class CGLTexture : public IRendererTexture {
      public:
        CGLTexture();
        virtual ~CGLTexture();

        virtual size_t                    id();
        virtual eTextureType              type();
        virtual void                      destroy();
        virtual eImageFitMode             fitMode();
        virtual Abyssutils::Math::Vector2D size();

        // attaches an async resource. uploads immediately if ready, otherwise
        // arms a listener that runs upload() when the resource is ready. self is
        // captured weakly so the listener becomes a no-op if the texture is
        // destroyed before the resource finishes loading.
        void                              attachAsync(WP<CGLTexture> self, ASP<Abyssgraphics::IAsyncResource> resource);

        eGLTextureType                    m_type      = TEXTURE_RGBA;
        GLenum                            m_target    = GL_TEXTURE_2D;
        bool                              m_allocated = false;
        GLuint                            m_texID     = 0;
        eImageFitMode                     m_fitMode   = IMAGE_FIT_MODE_STRETCH;
        Abyssutils::Math::Vector2D         m_size      = {};

        ASP<Abyssgraphics::IAsyncResource> m_resource;
        WP<SBackendLifetime>              m_lifetime;

        void                              upload();
        void                              uploadOnContext(WP<CGLTexture> self);
        void                              releaseFromRenderer();
        void                              allocate();
        void                              bind();
    };
};
