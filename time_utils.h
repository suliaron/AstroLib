#pragma once

#include "astro_types.h"  // DateTime

#include <cstddef>  // std::size_t

namespace astro {

    /**
     * @brief Converts a Gregorian calendar date and time to JD.
     *
     * @param date Gregorian calendar date and time.
     *
     * @return Julian Date (JD).
     */
    double toJD(const DateTime &date);

    /**
     * @brief Converts JD to a Gregorian calendar date and time.
     *
     * @param jd Julian Date (JD).
     *
     * @return Gregorian calendar date and time.
     */
    DateTime fromJD(double jd);

    /**
     * @brief Prints a Gregorian calendar date and time.
     *
     * @param date Date and time to print.
     * @param precision Number of decimal places used for seconds.
     */
    void print(const DateTime &date, int precision = 3);

    /**
     * @brief Generates output times with a constant number of points per decade.
     *
     * The schedule is intended primarily for long integrations of chaos
     * indicators, where output values are usually displayed on logarithmic
     * time scales.
     *
     * The first output time defines the scale of the first decade. Within
     * each decade, the requested number of output points is distributed
     * linearly. After all points of the current decade have been generated,
     * the time scale is multiplied by ten.
     *
     * For example, with
     *
     *     firstTime = 1.0e-4
     *     pointsPerDecade = 9
     *
     * the generated elapsed output times are
     *
     *     1e-4, 2e-4, ..., 9e-4,
     *     1e-3, 2e-3, ..., 9e-3,
     *     1e-2, 2e-2, ..., 9e-2, ...
     *
     * The generated values represent elapsed physical times measured from
     * the initial integration epoch.
     */
    class LogOutputSchedule {
       public:
        /**
         * @brief Constructs a logarithmic output schedule.
         *
         * @param firstTime First elapsed output time [day].
         * @param pointsPerDecade Number of output points in each time decade.
         */
        LogOutputSchedule(double firstTime, std::size_t pointsPerDecade) noexcept;

        /**
         * @brief Returns the next scheduled elapsed output time.
         *
         * @return Next elapsed output time [day].
         */
        double getNextTime() const noexcept
        {
            return next_time_;
        }

        /**
         * @brief Advances the schedule to the next output time.
         *
         * After the last point of the current decade, the time scale is
         * multiplied by ten and the first point of the next decade is selected.
         */
        void advance() noexcept;

       private:
        /**
         * @brief Number of output points in each decade.
         */
        std::size_t points_per_decade_;

        /**
         * @brief Index of the current point within the decade.
         *
         * The index starts from zero.
         */
        std::size_t point_index_ = 0;

        /**
         * @brief Time scale of the current decade [day].
         */
        double decade_scale_;

        /**
         * @brief Next elapsed output time [day].
         */
        double next_time_;
    };

}  // namespace astro