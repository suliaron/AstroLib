#pragma once

#include "astro_types.h"

namespace astro {

    /**
     * @brief Returns the square of a value.
     *
     * @tparam T Arithmetic type.
     * @param x Value to be squared.
     *
     * @return Square of @p x.
     */
    template <typename T>
    constexpr T sqr(T x)
    {
        return x * x;
    }

    /**
     * @brief Returns the cube of a value.
     *
     * @tparam T Arithmetic type.
     * @param x Value to be cubed.
     *
     * @return Cube of @p x.
     */
    template <typename T>
    constexpr T cube(T x)
    {
        return x * x * x;
    }

    /**
     * @brief Converts an angle from degrees to radians.
     *
     * @param deg Angle in degrees.
     *
     * @return Angle in radians.
     */
    constexpr double toRad(double deg)
    {
        return deg * pi / 180.0;
    }

    /**
     * @brief Converts an angle from radians to degrees.
     *
     * @param rad Angle in radians.
     *
     * @return Angle in degrees.
     */
    constexpr double toDeg(double rad)
    {
        return rad * 180.0 / pi;
    }
}  // namespace astro