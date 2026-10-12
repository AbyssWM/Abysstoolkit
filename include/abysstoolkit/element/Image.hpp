#pragma once

#include "Element.hpp"
#include "../types/SizeType.hpp"
#include "../types/ImageTypes.hpp"

#include <abyssgraphics/resource/AsyncResourceGatherer.hpp>
#include <abyssgraphics/resource/resources/ImageResource.hpp>

namespace Abysstoolkit {

    class IRendererTexture;
    struct SImageImpl;
    struct SImageData;
    class CImageElement;
    class ISystemIconDescription;

    class CImageBuilder {
      public:
        ~CImageBuilder() = default;

        static Abyssutils::Memory::CSharedPointer<CImageBuilder> begin();
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        path(std::string&&);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        icon(const Abyssutils::Memory::CSharedPointer<ISystemIconDescription>&);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        data(std::vector<uint8_t>&& data);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        a(float);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        fitMode(eImageFitMode);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        sync(bool);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        rounding(int);
        Abyssutils::Memory::CSharedPointer<CImageBuilder>        size(CDynamicSize&&);

        Abyssutils::Memory::CSharedPointer<CImageElement>        commence();

      private:
        Abyssutils::Memory::CWeakPointer<CImageBuilder> m_self;
        Abyssutils::Memory::CUniquePointer<SImageData>  m_data;
        Abyssutils::Memory::CWeakPointer<CImageElement> m_element;

        CImageBuilder() = default;

        friend class CImageElement;
    };

    class CImageElement : public IElement {
      public:
        virtual ~CImageElement() = default;

        Abyssutils::Memory::CSharedPointer<CImageBuilder> rebuild();
        virtual Abyssutils::Math::Vector2D                size();

      private:
        CImageElement(const SImageData& data);
        static Abyssutils::Memory::CSharedPointer<CImageElement> create(const SImageData& data);

        void                                                    replaceData(const SImageData& data);

        //
        virtual void                                     paint();
        virtual void                                     reposition(const Abyssutils::Math::CBox& box, const Abyssutils::Math::Vector2D& maxSize = {-1, -1});
        virtual std::optional<Abyssutils::Math::Vector2D> preferredSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> minimumSize(const Abyssutils::Math::Vector2D& parent);
        virtual std::optional<Abyssutils::Math::Vector2D> maximumSize(const Abyssutils::Math::Vector2D& parent);
        virtual bool                                     positioningDependsOnChild();

        void                                             renderTex();

        Abyssutils::Memory::CUniquePointer<SImageImpl>    m_impl;

        friend class CImageBuilder;
    };
};
