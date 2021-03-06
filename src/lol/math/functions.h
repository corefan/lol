//
//  Lol Engine
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//            © 2012—2013 Benjamin “Touky” Huet <huet.benjamin@gmail.com>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#pragma once

//
// Various maths functions
// -----------------------
//

#include <lol/math/constants.h>

#include <cmath>
#include <cstdio>
#include <algorithm>

#include <stdint.h>

namespace lol
{

/* This is OUR namespace. Don't let Windows headers fuck with it. */
#undef min
#undef max

/* Standard cmath functions */
static inline double sqrt(double const &x) { return std::sqrt(x); }
static inline float sqrt(float const &x) { return std::sqrt(x); }

static inline double cbrt(double const &x) { return std::cbrt(x); }
static inline float cbrt(float const &x) { return std::cbrt(x); }

static inline double exp(double const &x) { return std::exp(x); }
static inline float exp(float const &x) { return std::exp(x); }

static inline double sin(double const &x) { return std::sin(x); }
static inline double cos(double const &x) { return std::cos(x); }
static inline double tan(double const &x) { return std::tan(x); }
static inline float sin(float const &x) { return std::sin(x); }
static inline float cos(float const &x) { return std::cos(x); }
static inline float tan(float const &x) { return std::tan(x); }

static inline double asin(double const &x) { return std::asin(x); }
static inline double acos(double const &x) { return std::acos(x); }
static inline double atan(double const &x) { return std::atan(x); }
static inline float asin(float const &x) { return std::asin(x); }
static inline float acos(float const &x) { return std::acos(x); }
static inline float atan(float const &x) { return std::atan(x); }

static inline double atan2(double const &y, double const &x)
{
    return std::atan2(y, x);
}

static inline float atan2(float const &y, float const &x)
{
    return std::atan2(y, x);
}

static inline double pow(double const &x, double const &y)
{
    return std::pow(x, y);
}

static inline float pow(float const &x, float const &y)
{
    return std::pow(x, y);
}

/* Our extensions */
static inline void sincos(double const &x, double *s, double *c)
{
    *s = std::sin(x);
    *c = std::cos(x);
}

static inline void sincos(float const &x, float *s, float *c)
{
    *s = std::sin(x);
    *c = std::cos(x);
}

/* Inherited from GLSL */
static inline float degrees(float radians)
{
    return radians * (180.0f / F_PI);
}

static inline double degrees(double radians)
{
    return radians * (180.0 / D_PI);
}

static inline ldouble degrees(ldouble radians)
{
    return radians * (180.0L / LD_PI);
}

static inline float radians(float degrees)
{
    return degrees * (F_PI / 180.0f);
}

static inline double radians(double degrees)
{
    return degrees * (D_PI / 180.0);
}

static inline ldouble radians(ldouble degrees)
{
    return degrees * (LD_PI / 180.0L);
}

/* The integer versions return floating point values. This avoids nasty
 * surprises when calling radians(180) instead of radians(180.0). */
static inline float   degrees(int8_t x)   { return degrees(float(x)); }
static inline float   degrees(uint8_t x)  { return degrees(float(x)); }
static inline float   degrees(int16_t x)  { return degrees(float(x)); }
static inline float   degrees(uint16_t x) { return degrees(float(x)); }
static inline double  degrees(int32_t x)  { return degrees(double(x)); }
static inline double  degrees(uint32_t x) { return degrees(double(x)); }
static inline ldouble degrees(int64_t x)  { return degrees(ldouble(x)); }
static inline ldouble degrees(uint64_t x) { return degrees(ldouble(x)); }

static inline float   radians(int8_t x)   { return radians(float(x)); }
static inline float   radians(uint8_t x)  { return radians(float(x)); }
static inline float   radians(int16_t x)  { return radians(float(x)); }
static inline float   radians(uint16_t x) { return radians(float(x)); }
static inline double  radians(int32_t x)  { return radians(double(x)); }
static inline double  radians(uint32_t x) { return radians(double(x)); }
static inline ldouble radians(int64_t x)  { return radians(ldouble(x)); }
static inline ldouble radians(uint64_t x) { return radians(ldouble(x)); }

static inline float mix(float const &a, float const &b, float const &x)
{
    return a + (b - a) * x;
}

static inline double mix(double const &a, double const &b, double const &x)
{
    return a + (b - a) * x;
}

static inline ldouble mix(ldouble const &a, ldouble const &b, ldouble const &x)
{
    return a + (b - a) * x;
}

/* Inherited from HLSL */
static inline float lerp(float const &a, float const &b, float const &x)
{
    return mix(a, b, x);
}

static inline double lerp(double const &a, double const &b, double const &x)
{
    return mix(a, b, x);
}

static inline ldouble lerp(ldouble const &a, ldouble const &b, ldouble const &x)
{
    return mix(a, b, x);
}

/* These accelerated functions will be merged into the above, one day */
double lol_sin(double);
double lol_cos(double);
double lol_tan(double);
void lol_sincos(double, double*, double*);
void lol_sincos(float, float*, float*);
double lol_asin(double);
double lol_acos(double);
double lol_atan(double);
double lol_atan2(double, double);

/* C++ doesn't define abs() and fmod() for all types; we add these for
 * convenience to avoid adding complexity to vector.h. */
static inline int8_t abs(int8_t x) { return std::abs(x); }
static inline uint8_t abs(uint8_t x) { return x; }
static inline int16_t abs(int16_t x) { return std::abs(x); }
static inline uint16_t abs(uint16_t x) { return x; }
static inline int32_t abs(int32_t x) { return std::abs(x); }
static inline uint32_t abs(uint32_t x) { return x; }
#if defined __native_client__ || defined __ANDROID__
/* The pepper 19 toolchain doesn't provide abs() for long long int. */
static inline int64_t abs(int64_t x) { return x > 0 ? x : -x; }
#else
static inline int64_t abs(int64_t x) { return std::abs(x); }
#endif
static inline uint64_t abs(uint64_t x) { return x; }
static inline float abs(float x) { return std::abs(x); }
static inline double abs(double x) { return std::abs(x); }
static inline ldouble abs(ldouble x) { return std::abs(x); }

static inline uint8_t fmod(uint8_t x, uint8_t y) { return x % y; }
static inline int8_t fmod(int8_t x, int8_t y) { return x % y; }
static inline uint16_t fmod(uint16_t x, uint16_t y) { return x % y; }
static inline int16_t fmod(int16_t x, int16_t y) { return x % y; }
static inline uint32_t fmod(uint32_t x, uint32_t y) { return x % y; }
static inline int32_t fmod(int32_t x, int32_t y) { return x % y; }
static inline uint64_t fmod(uint64_t x, uint64_t y) { return x % y; }
static inline int64_t fmod(int64_t x, int64_t y) { return x % y; }
static inline float fmod(float x, float y) { return std::fmod(x, y); }
static inline double fmod(double x, double y) { return std::fmod(x, y); }
static inline ldouble fmod(ldouble x, ldouble y) { return std::fmod(x, y); }

static inline uint8_t floor(uint8_t x) { return x; }
static inline int8_t floor(int8_t x) { return x; }
static inline uint16_t floor(uint16_t x) { return x; }
static inline int16_t floor(int16_t x) { return x; }
static inline uint32_t floor(uint32_t x) { return x; }
static inline int32_t floor(int32_t x) { return x; }
static inline uint64_t floor(uint64_t x) { return x; }
static inline int64_t floor(int64_t x) { return x; }
static inline float floor(float x) { return std::floor(x); }
static inline double floor(double x) { return std::floor(x); }
static inline ldouble floor(ldouble x) { return std::floor(x); }

static inline uint8_t ceil(uint8_t x) { return x; }
static inline int8_t ceil(int8_t x) { return x; }
static inline uint16_t ceil(uint16_t x) { return x; }
static inline int16_t ceil(int16_t x) { return x; }
static inline uint32_t ceil(uint32_t x) { return x; }
static inline int32_t ceil(int32_t x) { return x; }
static inline uint64_t ceil(uint64_t x) { return x; }
static inline int64_t ceil(int64_t x) { return x; }
static inline float ceil(float x) { return std::ceil(x); }
static inline double ceil(double x) { return std::ceil(x); }
static inline ldouble ceil(ldouble x) { return std::ceil(x); }

static inline uint8_t round(uint8_t x) { return x; }
static inline int8_t round(int8_t x) { return x; }
static inline uint16_t round(uint16_t x) { return x; }
static inline int16_t round(int16_t x) { return x; }
static inline uint32_t round(uint32_t x) { return x; }
static inline int32_t round(int32_t x) { return x; }
static inline uint64_t round(uint64_t x) { return x; }
static inline int64_t round(int64_t x) { return x; }
static inline float round(float x) { return std::round(x); }
static inline double round(double x) { return std::round(x); }
static inline ldouble round(ldouble x) { return std::round(x); }

#define LOL_GENERIC_FUNC(T) \
    static inline T sq(T x) { return x * x; } \
    static inline T fract(T x) { return x - lol::floor(x); } \
    static inline T min(T x, T y) { return std::min(x, y); } \
    static inline T max(T x, T y) { return std::max(x, y); } \
    static inline T clamp(T x, T y, T z) { return min(max(x, y), z); } \
    static inline T saturate(T x) { return min(max(x, (T)0), (T)1); }

#define LOL_GENERIC_FUNC_SIGNED(T) \
    LOL_GENERIC_FUNC(T) \
    static inline T sign(T x) { return (T)(((T)0 < x) - (x < (T)0)); }

#define LOL_GENERIC_FUNC_UNSIGNED(T) \
    LOL_GENERIC_FUNC(T) \
    static inline T sign(T x) { return (T)((T)0 < x); }

LOL_GENERIC_FUNC_UNSIGNED(uint8_t)
LOL_GENERIC_FUNC_SIGNED(int8_t)
LOL_GENERIC_FUNC_UNSIGNED(uint16_t)
LOL_GENERIC_FUNC_SIGNED(int16_t)
LOL_GENERIC_FUNC_UNSIGNED(uint32_t)
LOL_GENERIC_FUNC_SIGNED(int32_t)
LOL_GENERIC_FUNC_UNSIGNED(uint64_t)
LOL_GENERIC_FUNC_SIGNED(int64_t)
LOL_GENERIC_FUNC_SIGNED(float)
LOL_GENERIC_FUNC_SIGNED(double)
LOL_GENERIC_FUNC_SIGNED(ldouble)
#undef LOL_GENERIC_FUNC

} /* namespace lol */

