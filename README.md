# AstroLib

**AstroLib** is a small C++ utility library for astronomical and celestial-mechanics calculations.

It provides common data types, mathematical helpers, orbital-element transformations, time-conversion utilities, and output-time scheduling used by the **Arch of Chaos** project.

Repository: <https://github.com/suliaron/AstroLib>

---

## Main features

- Common astronomical data structures.
- Basic mathematical helper functions.
- Conversion between osculating orbital elements and Cartesian state vectors.
- Time conversion utilities used by normalized dynamical models.
- Logarithmic output scheduling for long integrations.
- C++ source designed to remain compatible with both Microsoft Visual C++ and GCC-style compilers.

---

## Repository contents

```text
astro_types.h
    Common astronomical data types.

math_utils.h
    Mathematical helper functions.

orbit.h / orbit.cpp
    Orbital-element and Cartesian-state transformations.

time_utils.h / time_utils.cpp
    Time-conversion utilities and logarithmic output scheduling.

AstroLib.vcxproj
AstroLib.vcxproj.filters
    Microsoft Visual Studio project files.
```

---

## Logarithmic output scheduling

`time_utils.h` / `time_utils.cpp` provide the `LogOutputSchedule` class for generating output times with a constant number of samples per time decade.

For example,

```cpp
astro::LogOutputSchedule schedule(1.0e-4, 9);
```

generates elapsed output times of the form

```text
1e-4, 2e-4, ..., 9e-4,
1e-3, 2e-3, ..., 9e-3,
1e-2, ...
```

This is useful for long integrations whose results are typically displayed on logarithmic time axes.

---

## Building

The repository includes a Microsoft Visual Studio C++ project:

```text
AstroLib.vcxproj
```

The library can be built as part of a Visual Studio solution and linked to applications that use the AstroLib headers and compiled library.

Typical Visual Studio configurations are:

```text
Debug x64
Release x64
```

Build products and user-specific Visual Studio files are excluded from version control through `.gitignore`.

---

## Use with Arch of Chaos

AstroLib is used by **Arch of Chaos** for shared astronomical functionality, including:

- orbital state calculations;
- mathematical helper functions;
- physical/dimensionless time conversions;
- logarithmic output scheduling.

When building Arch of Chaos, the AstroLib project should be available and built for the same target configuration and platform.

---

## Development status

AstroLib is currently a compact research-support library developed primarily for the Arch of Chaos project.

The API may evolve as additional functionality is moved from application-specific code into reusable library components.

---

## Author

**Dr. Áron Süli**  
Eötvös Loránd University (ELTE)

---

## License

No open-source license has been added yet.

Until a license is provided, the repository source code remains copyrighted and should not be assumed to permit redistribution or reuse beyond what copyright law otherwise allows.
