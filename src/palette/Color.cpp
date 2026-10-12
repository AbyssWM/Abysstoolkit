#include <abysstoolkit/palette/Color.hpp>
#include "../Macros.hpp"
#include "../helpers/Memory.hpp"
#include <abyssutils/memory/Casts.hpp>

using namespace Abysstoolkit;
using namespace Abyssutils::Memory;

#define ALPHA(c) ((double)(((c) >> 24) & 0xff) / 255.0)
#define RED(c)   ((double)(((c) >> 16) & 0xff) / 255.0)
#define GREEN(c) ((double)(((c) >> 8) & 0xff) / 255.0)
#define BLUE(c)  ((double)(((c)) & 0xff) / 255.0)

CAbyssColor::CAbyssColor() {
    ;
}

CAbyssColor::CAbyssColor(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) {
    okLab = Abyssgraphics::CColor(Abyssgraphics::CColor::SSRGB{.r = r, .g = g, .b = b}).asOkLab();
}

CAbyssColor::CAbyssColor(uint64_t hex) : r(RED(hex)), g(GREEN(hex)), b(BLUE(hex)), a(ALPHA(hex)) {
    okLab = Abyssgraphics::CColor(Abyssgraphics::CColor::SSRGB{.r = r, .g = g, .b = b}).asOkLab();
}

CAbyssColor::CAbyssColor(const Abyssgraphics::CColor& color, float a_) : a(a_) {
    const auto SRGB = color.asRgb();
    r               = SRGB.r;
    g               = SRGB.g;
    b               = SRGB.b;

    okLab = color.asOkLab();
}

bool CAbyssColor::operator==(const CAbyssColor& c2) const {
    return c2.r == r && c2.g == g && c2.b == b && c2.a == a;
}

CAbyssColor CAbyssColor::operator-(const CAbyssColor& c2) const {
    RASSERT(false, "CAbyssColor: - is a STUB");
    return {};
}

CAbyssColor CAbyssColor::operator+(const CAbyssColor& c2) const {
    RASSERT(false, "CAbyssColor: + is a STUB");
    return {};
}

CAbyssColor CAbyssColor::operator*(const float& c2) const {
    RASSERT(false, "CAbyssColor: * is a STUB");
    return {};
}

uint32_t CAbyssColor::getAsHex() const {
    return ((uint32_t)(a * 255.f) * 0x1000000) + ((uint32_t)(r * 255.f) * 0x10000) + ((uint32_t)(g * 255.f) * 0x100) + ((uint32_t)(b * 255.f) * 0x1);
}

Abyssgraphics::CColor::SSRGB CAbyssColor::asRGB() const {
    return {.r = r, .g = g, .b = b};
}

Abyssgraphics::CColor::SOkLab CAbyssColor::asOkLab() const {
    return okLab;
}

Abyssgraphics::CColor::SHSL CAbyssColor::asHSL() const {
    return Abyssgraphics::CColor(okLab).asHSL();
}

CAbyssColor CAbyssColor::stripA() const {
    return {sc<float>(r), sc<float>(g), sc<float>(b), 1.F};
}

CAbyssColor CAbyssColor::brighten(float coeff) const {
    return {sc<float>(r * (1.F + coeff)), sc<float>(g * (1.F + coeff)), sc<float>(b * (1.F + coeff)), sc<float>(a)};
}

CAbyssColor CAbyssColor::darken(float coeff) const {
    return {sc<float>(r * (1.F - coeff)), sc<float>(g * (1.F - coeff)), sc<float>(b * (1.F - coeff)), sc<float>(a)};
}

CAbyssColor CAbyssColor::mix(const CAbyssColor& with, float coeff) const {
    const auto C = std::clamp(coeff, 0.F, 1.F);

    const auto A = asOkLab();
    const auto B = with.asOkLab();

    return {
        Abyssgraphics::CColor::SOkLab{
            .l = (A.l * (1.F - C)) + (B.l * C),
            .a = (A.a * (1.F - C)) + (B.a * C),
            .b = (A.b * (1.F - C)) + (B.b * C),
        },
        sc<float>((a * (1.F - C)) + (with.a * C)),
    };
}
