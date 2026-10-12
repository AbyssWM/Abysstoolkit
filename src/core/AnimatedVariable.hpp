#pragma once

#include <abysstoolkit/palette/Color.hpp>
#include <abyssutils/animation/AnimatedVariable.hpp>
#include <abyssutils/math/Vector2D.hpp>

#include "../helpers/Memory.hpp"

namespace Abysstoolkit {

    enum eAnimatedVarType : int8_t {
        AVARTYPE_INVALID = -1,
        AVARTYPE_FLOAT,
        AVARTYPE_VECTOR,
        AVARTYPE_COLOR,
        AVARTYPE_GRADIENT
    };

    // Utility to bind a type with its corresponding eAnimatedVarType
    template <class T>
    // NOLINTNEXTLINE(readability-identifier-naming)
    struct STypeToAnimatedVarType_t {
        static constexpr eAnimatedVarType value = AVARTYPE_INVALID;
    };

    template <>
    struct STypeToAnimatedVarType_t<float> {
        static constexpr eAnimatedVarType value = AVARTYPE_FLOAT;
    };

    template <>
    struct STypeToAnimatedVarType_t<Abyssutils::Math::Vector2D> {
        static constexpr eAnimatedVarType value = AVARTYPE_VECTOR;
    };

    template <>
    struct STypeToAnimatedVarType_t<CAbyssColor> {
        static constexpr eAnimatedVarType value = AVARTYPE_COLOR;
    };

    // TODO:
    // template <>
    // struct STypeToAnimatedVarType_t<CGradientValueData> {
    //     static constexpr eAnimatedVarType value = AVARTYPE_GRADIENT;
    // };

    template <class T>
    inline constexpr eAnimatedVarType typeToeAnimatedVarType = STypeToAnimatedVarType_t<T>::value;

    // Utility to define a concept as a list of possible type
    template <class T, class... U>
    concept OneOf = (... or std::same_as<T, U>);

    // Concept to describe which type can be placed into CAnimatedVariable
    // This is mainly to get better errors if we put a type that's not supported
    // Otherwise template errors are ugly
    template <class T>
    concept Animable = OneOf<T, Abyssutils::Math::Vector2D, float, CAbyssColor /*, CGradientValueData*/>;

    struct SAnimationContext {};

    template <Animable VarType>
    using CAnimatedVariable = Abyssutils::Animation::CGenericAnimatedVariable<VarType, SAnimationContext>;

    template <Animable VarType>
    using PHLANIMVAR = UP<CAnimatedVariable<VarType>>;

    template <Animable VarType>
    using PHLANIMVARREF = WP<CAnimatedVariable<VarType>>;
}
