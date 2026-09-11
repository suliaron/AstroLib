#include "math_utils.h"  // sqr, cube utility functions
#include "orbit.h"       // Orbit conversion function declarations

#include <cmath>   // std::abs, std::atan2, std::cos, std::sin, std::sqrt
#include <cstdio>  // printf

namespace {

    /**
     * @brief Solves the elliptic Kepler equation for the eccentric anomaly.
     *
     * Solves the equation
     *
     *     E - e * sin(E) = M
     *
     * using Newton's method.
     *
     * @param M Mean anomaly [rad].
     * @param e Eccentricity [-].
     * @param tolerance Convergence tolerance.
     *
     * @return Eccentric anomaly [rad].
     */
    double solveKeplerEquation(double M, double e, double tolerance)
    {
        double E = M + e * std::sin(M) / (1.0 - std::sin(M + e) + std::sin(M));
        double dE;

        do {
            dE = (E - e * std::sin(E) - M) / (1.0 - e * std::cos(E));
            E -= dE;
        } while (std::abs(dE) >= tolerance);

        return E;
    }

}  // namespace

namespace astro {

    State calcState(double mu, double t, const OrbitalElements &oe, double tolerance)
    {
        const double a  = oe.a;
        const double e  = oe.e;
        const double i  = oe.i;
        const double om = oe.omega;
        const double Om = oe.Omega;

        const double n = std::sqrt(mu / cube(a));
        const double M = n * (t - oe.tau);
        const double E = solveKeplerEquation(M, e, tolerance);

        const double sq  = std::sqrt(1.0 - sqr(e));
        const double den = 1.0 - e * std::cos(E);

        /*
        const double xi     = a * (std::cos(E) - e);
        const double eta    = a * sqrt(1 - e * e) * std::sin(E);
        const double xiDot  = -a * n * std::sin(E) / (1 - e * std::cos(E));
        const double etaDot = a * n * sqrt(1 - e * e) * std::cos(E) / (1 - e * std::cos(E));
        */

        const double xi     = a * (std::cos(E) - e);
        const double eta    = a * sq * std::sin(E);
        const double xiDot  = -a * n * std::sin(E) / den;
        const double etaDot = a * n * sq * std::cos(E) / den;

        const Vector P{std::cos(om) * std::cos(Om) - std::sin(om) * std::sin(Om) * std::cos(i),
                       std::cos(om) * std::sin(Om) + std::sin(om) * std::cos(Om) * std::cos(i),
                       std::sin(om) * std::sin(i)};

        const Vector Q{-std::sin(om) * std::cos(Om) - std::cos(om) * std::sin(Om) * std::cos(i),
                       -std::sin(om) * std::sin(Om) + std::cos(om) * std::cos(Om) * std::cos(i),
                       std::cos(om) * std::sin(i)};

        State state;

        state.r.x = P.x * xi + Q.x * eta;
        state.r.y = P.y * xi + Q.y * eta;
        state.r.z = P.z * xi + Q.z * eta;

        state.v.x = P.x * xiDot + Q.x * etaDot;
        state.v.y = P.y * xiDot + Q.y * etaDot;
        state.v.z = P.z * xiDot + Q.z * etaDot;

        return state;
    }

    OrbitalElements calcOrbitalElements(double mu, double t, const State &state)
    {
        const Vector &r = state.r;
        const Vector &v = state.v;

        // Specific angular momentum vector.
        const Vector c{
            r.y * v.z - r.z * v.y, 
            r.z * v.x - r.x * v.z, 
            r.x * v.y - r.y * v.x
        };

        const double cNorm = std::sqrt(sqr(c.x) + sqr(c.y) + sqr(c.z));
        const double rNorm = std::sqrt(sqr(r.x) + sqr(r.y) + sqr(r.z));
        const double v2 = sqr(v.x) + sqr(v.y) + sqr(v.z);

        // Specific orbital energy.
        const double h = 0.5 * v2 - mu / rNorm;

        // Laplace-Runge-Lenz vector multiplied by mu.
        const Vector l{
            v.y * c.z - v.z * c.y - mu * r.x / rNorm, 
            v.z * c.x - v.x * c.z - mu * r.y / rNorm,
            v.x * c.y - v.y * c.x - mu * r.z / rNorm
        };

        const double lNorm = std::sqrt(sqr(l.x) + sqr(l.y) + sqr(l.z));

        OrbitalElements oe{};

        // Semimajor axis.
        oe.a = -mu / (2.0 * h);

        // Mean motion.
        const double n = std::sqrt(mu / cube(oe.a));

        // Eccentricity.
        oe.e = std::sqrt(1.0 + 2.0 * h * sqr(cNorm / mu));

        // Eccentric anomaly.
        const double ec = 1.0 - rNorm / oe.a;

        const double es = (r.x * v.x + r.y * v.y + r.z * v.z) / (n * sqr(oe.a));

        double E = std::atan2(es, ec);

        if (E < 0.0) {
            E += 2.0 * pi;
        }

        // Mean anomaly.
        const double M = E - oe.e * std::sin(E);

        // Time of periapsis passage.
        oe.tau = t - M / n;

        // Inclination.
        oe.i = std::atan2(std::sqrt(sqr(c.x) + sqr(c.y)), c.z);

        // Compute the longitude of the ascending node and the argument
        // of periapsis. The coplanar case must be handled separately
        // because the ascending node is undefined for i = 0.
        if (std::abs(std::sin(oe.i)) < 1.0e-12) {
            // For a coplanar orbit, choose Omega = 0 by convention.
            oe.Omega = 0.0;

            // In the coplanar case, omega represents the longitude
            // of periapsis.
            oe.omega = std::atan2(l.y, l.x);

            if (oe.omega < 0.0) {
                oe.omega += 2.0 * pi;
            }

        } else {
            // Longitude of the ascending node.
            const double sOm = c.x / (cNorm * std::sin(oe.i));

            const double cOm = -c.y / (cNorm * std::sin(oe.i));

            oe.Omega = std::atan2(sOm, cOm);

            if (oe.Omega < 0.0) {
                oe.Omega += 2.0 * pi;
            }

            // Argument of periapsis.
            const double som = (l.y * cOm - l.x * sOm) / (lNorm * std::cos(oe.i));

            const double com = (l.x * cOm + l.y * sOm) / lNorm;

            oe.omega = std::atan2(som, com);

            if (oe.omega < 0.0) {
                oe.omega += 2.0 * pi;
            }
        }

        return oe;
    }

    void print(const Vector &v, int precision)
    {
        printf("x = %+.*f\n", precision, v.x);
        printf("y = %+.*f\n", precision, v.y);
        printf("z = %+.*f\n", precision, v.z);
    }

    void print(const State &state, int precision)
    {
        printf("Position:\n");
        print(state.r, precision);

        printf("Velocity:\n");
        print(state.v, precision);
    }

    void print(const OrbitalElements &elements, int precision)
    {
        printf("a     = %.*f AU\n", precision, elements.a);
        printf("e     = %.*f\n", precision, elements.e);
        printf("i     = %.*f deg\n", precision, toDeg(elements.i));
        printf("omega = %.*f deg\n", precision, toDeg(elements.omega));
        printf("Omega = %.*f deg\n", precision, toDeg(elements.Omega));
        printf("tau   = %.*f JD\n", precision, elements.tau);
    }
}  // namespace astro
