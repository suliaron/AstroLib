#pragma once

namespace astro {

    inline constexpr double k = 0.01720209895;
    inline constexpr double pi = 3.141592653589793;

    struct Vector
    {
        double x;
        double y;
        double z;
    };

    /**
     * @brief Represents a Cartesian state vector.
     *
     * Contains the Cartesian position and velocity vectors of an object.
     */
    struct State
    {
        Vector r;  /**< Position vector [AU]. */
        Vector v;  /**< Velocity vector [AU/day]. */
    };

    /**
     * @brief Represents the classical Keplerian orbital elements.
     *
     * Describes a Keplerian orbit using the semimajor axis, eccentricity,
     * inclination, argument of periapsis, longitude of the ascending node,
     * and time of periapsis passage.
     *
     * All angular quantities are expressed in radians.
     */
    struct OrbitalElements
    {
        double a;      /**< Semimajor axis [AU]. */
        double e;      /**< Eccentricity [-]. */
        double i;      /**< Inclination [rad]. */
        double omega;  /**< Argument of pericenter [rad]. */
        double Omega;  /**< Longitude of the ascending node [rad]. */
        double tau;    /**< Time of pericenter passage [JD]. */
    };

    /**
     * @brief Represents a Gregorian calendar date and time.
     */
    struct DateTime
    {
        int year;       /**< Year. */
        int month;      /**< Month [1-12]. */
        int day;        /**< Day [1-31]. */
        int hour;       /**< Hour [0-23]. */
        int minute;     /**< Minute [0-59]. */
        double second;  /**< Second [0-60). */
    };
}  // namespace astro