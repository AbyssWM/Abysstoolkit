#pragma once

#include <cstdint>
#include <abyssgraphics/color/Color.hpp>

namespace Abysstoolkit {
    class CAbyssColor {
      public:
        CAbyssColor();
        CAbyssColor(float r, float g, float b, float a = 1.F);
        CAbyssColor(const Abyssgraphics::CColor& col, float a);
        CAbyssColor(uint64_t);

        // AR32
        uint32_t                     getAsHex() const;
        Abyssgraphics::CColor::SSRGB  asRGB() const;
        Abyssgraphics::CColor::SOkLab asOkLab() const;
        Abyssgraphics::CColor::SHSL   asHSL() const;
        CAbyssColor                   stripA() const;

        CAbyssColor                   brighten(float coeff) const;
        CAbyssColor                   darken(float coeff) const;
        CAbyssColor                   mix(const CAbyssColor& with, float coeff) const;

        //
        bool operator==(const CAbyssColor& c2) const;

        // stubs for the AnimationMgr
        CAbyssColor operator-(const CAbyssColor& c2) const;
        CAbyssColor operator+(const CAbyssColor& c2) const;
        CAbyssColor operator*(const float& c2) const;

        double     r = 0, g = 0, b = 0, a = 0;

      private:
        Abyssgraphics::CColor::SOkLab okLab; // cache for the OkLab representation
    };
}