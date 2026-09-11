#include "time_utils.h"  // Date and JD conversion functions

#include <cmath>   // std::floor
#include <cstdio>  // printf

namespace astro {

    double toJD(const DateTime &date)
    {
        int y = date.year;
        int m = date.month;

        if (m <= 2) {
            --y;
            m += 12;
        }

        const int A = y / 100;
        const int B = 2 - A + A / 4;

        const double d = static_cast<double>(date.day) + static_cast<double>(date.hour) / 24.0 +
                         static_cast<double>(date.minute) / 1440.0 + date.second / 86400.0;

        const double jd = std::floor(365.25 * (y + 4716)) + std::floor(30.6001 * (m + 1)) + d + B - 1524.5;

        return jd;
    }

    DateTime fromJD(double jd)
    {
        const double x = jd + 0.5;
        const double Z = std::floor(x);
        const double F = x - Z;

        double A = Z;

        if (Z >= 2299161.0) {
            const double alpha = std::floor((Z - 1867216.25) / 36524.25);

            A = Z + 1.0 + alpha - std::floor(alpha / 4.0);
        }

        const double B = A + 1524.0;
        const double C = std::floor((B - 122.1) / 365.25);
        const double D = std::floor(365.25 * C);
        const double E = std::floor((B - D) / 30.6001);

        const double d = B - D - std::floor(30.6001 * E) + F;

        DateTime date{};

        date.day = static_cast<int>(std::floor(d));

        if (E < 14.0)
            date.month = static_cast<int>(E) - 1;
        else
            date.month = static_cast<int>(E) - 13;

        if (date.month > 2)
            date.year = static_cast<int>(C) - 4716;
        else
            date.year = static_cast<int>(C) - 4715;

        double f = d - std::floor(d);

        f *= 24.0;
        date.hour = static_cast<int>(std::floor(f));

        f           = (f - date.hour) * 60.0;
        date.minute = static_cast<int>(std::floor(f));

        date.second = (f - date.minute) * 60.0;

        return date;
    }

    void print(const DateTime &date, int precision)
    {
        printf("%04d-%02d-%02d %02d:%02d:%0*.*f\n", date.year, date.month, date.day, date.hour, date.minute,
               precision + 3, precision, date.second);
    }

    LogOutputSchedule::LogOutputSchedule(double firstTime, std::size_t pointsPerDecade) noexcept
    {
        points_per_decade_ = pointsPerDecade;
        decade_scale_      = firstTime;
        next_time_         = firstTime;
    }

    void LogOutputSchedule::advance() noexcept
    {
        ++point_index_;

        if (point_index_ >= points_per_decade_) {
            point_index_ = 0;
            decade_scale_ *= 10.0;
            next_time_ = decade_scale_;
            return;
        }

        const double fraction = static_cast<double>(point_index_) / static_cast<double>(points_per_decade_);

        constexpr double DECADE_FACTOR = 10.0;

        next_time_ = decade_scale_ * (1.0 + (DECADE_FACTOR - 1.0) * fraction);

    }
}  // namespace astro