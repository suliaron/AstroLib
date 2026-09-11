#pragma once

#include "astro_types.h"

namespace astro {

    /**
     * @brief Computes the Cartesian state from Keplerian orbital elements.
     *
     * Calculates the position and velocity vectors at the specified epoch
     * from the Keplerian orbital elements.
     *
     * The mean anomaly at epoch @p t is computed from the time of periapsis
     * passage stored in @p elements.
     *
     * @param mu Gravitational parameter [AU^3/day^2].
     * @param t Epoch [JD].
     * @param elements Keplerian orbital elements.
     * @param tolerance Convergence tolerance for solving Kepler's equation.
     *
     * @return Cartesian state containing the position and velocity vectors.
     */
    State calcState(double mu,
        double t,
        const OrbitalElements& elements,
        double tolerance = 1e-8);

    /**
     * @brief Computes Keplerian orbital elements from a Cartesian state.
     *
     * Determines the semimajor axis, eccentricity, inclination, argument of
     * periapsis, longitude of the ascending node, and time of periapsis passage
     * from the Cartesian position and velocity vectors at the specified epoch.
     *
     * Angular elements are returned in radians.
     *
     * @param mu Gravitational parameter [AU^3/day^2].
     * @param t Epoch [JD].
     * @param state Cartesian state containing position and velocity vectors.
     *
     * @return Keplerian orbital elements.
     */
    OrbitalElements calcOrbitalElements(double mu,
        double t,
        const State& state);

    /**
     * @brief Prints a three-dimensional vector.
     *
     * @param v Vector to print.
     * @param precision Number of decimal places.
     */
    void print(const Vector& v, int precision = 15);

    /**
     * @brief Prints a Cartesian state.
     *
     * @param state Cartesian state to print.
     * @param precision Number of decimal places.
     */
    void print(const State& state, int precision = 15);

    /**
     * @brief Prints Keplerian orbital elements.
     *
     * Angular elements are printed in degrees.
     *
     * @param elements Orbital elements to print.
     * @param precision Number of decimal places.
     */
    void print(const OrbitalElements& elements, int precision = 10);
}  // namespace astro