//
//  Lol Engine — Unit tests for trigonometric functions
//
//  Copyright © 2010—2015 Sam Hocevar <sam@hocevar.net>
//
//  Lol Engine is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#include <lol/engine-internal.h>

#include <cmath>

#include <lolunit.h>

namespace lol
{

lolunit_declare_fixture(trig_test)
{
    lolunit_declare_test(angle_conversions)
    {
        lolunit_assert_doubles_equal(D_PI, radians(180.0), 1e-5);
        lolunit_assert_doubles_equal(D_PI_2, radians(90.0), 1e-5);

        lolunit_assert_doubles_equal(F_PI, radians(180.0f), 1e-5f);
        lolunit_assert_doubles_equal(F_PI_2, radians(90.0f), 1e-5f);

        lolunit_assert_doubles_equal(180.0, degrees(D_PI), 1e-5);
        lolunit_assert_doubles_equal(90.0, degrees(D_PI_2), 1e-5);

        lolunit_assert_doubles_equal(180.0f, degrees(F_PI), 1e-5f);
        lolunit_assert_doubles_equal(90.0f, degrees(F_PI_2), 1e-5f);
    }

    lolunit_declare_test(integer_angle_conversions)
    {
        lolunit_assert_doubles_equal(radians(90), radians(90.0), 1e-5);

        lolunit_assert_doubles_equal(radians((uint8_t)90),  radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int8_t)90),   radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((uint16_t)90), radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int16_t)90),  radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((uint32_t)90), radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int32_t)90),  radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((uint64_t)90), radians(90.0), 1e-5);
        lolunit_assert_doubles_equal(radians((int64_t)90),  radians(90.0), 1e-5);

        lolunit_assert_doubles_equal(degrees(1), degrees(1.0), 1e-5);

        lolunit_assert_doubles_equal(degrees((uint8_t)1),  degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((int8_t)1),   degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((uint16_t)1), degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((int16_t)1),  degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((uint32_t)1), degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((int32_t)1),  degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((uint64_t)1), degrees(1.0), 1e-5);
        lolunit_assert_doubles_equal(degrees((int64_t)1),  degrees(1.0), 1e-5);
    }

    lolunit_declare_test(sin)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_sin(f);
#else
            double a = std::sin(f);
#endif
            double b = lol_sin(f);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_sin(f);
#else
            double a = std::sin(f);
#endif
            double b = lol_sin(f);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }
    }

    lolunit_declare_test(cos)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_cos(f);
#else
            double a = std::cos(f);
#endif
            double b = lol_cos(f);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_cos(f);
#else
            double a = std::cos(f);
#endif
            double b = lol_cos(f);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }
    }

    lolunit_declare_test(sin_cos)
    {
        using std::fabs;

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 1000.0);
#if defined __GNUC__ && !defined __SNC__
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
#else
            double a1 = std::sin(f);
            double a2 = std::cos(f);
#endif
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a1, b1, fabs(f) * 1e-11);
            lolunit_assert_doubles_equal(a2, b2, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a1 = __builtin_sin(f);
            double a2 = __builtin_cos(f);
#else
            double a1 = std::sin(f);
            double a2 = std::cos(f);
#endif
            double b1, b2;
            lol_sincos(f, &b1, &b2);
            lolunit_set_context(f);
            lolunit_assert_doubles_equal(a1, b1, fabs(f) * 1e-11);
            lolunit_assert_doubles_equal(a2, b2, fabs(f) * 1e-11);
        }
    }

    lolunit_declare_test(tan)
    {
        using std::fabs;

        for (int i = -100000; i < 100000; i++)
        {
            double f = (double)i * (1.0 / 10000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_tan(f);
#else
            double a = std::tan(f);
#endif
            double b = lol_tan(f);
            lolunit_set_context(f);
            if (fabs(a) > 1e4)
                lolunit_assert_doubles_equal(a, b, fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
                lolunit_assert_doubles_equal(a, b, fabs(a) * 1e-11);
            else
                lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }

        for (int i = -10000; i < 10000; i++)
        {
            double f = (double)i * (1.0 / 100000.0);
#if defined __GNUC__ && !defined __SNC__
            double a = __builtin_tan(f);
#else
            double a = std::tan(f);
#endif
            double b = lol_tan(f);
            lolunit_set_context(f);
            if (fabs(a) > 1e4)
                lolunit_assert_doubles_equal(a, b, fabs(a) * fabs(a) * 1e-11);
            else if (fabs(a) > 1.0)
                lolunit_assert_doubles_equal(a, b, fabs(a) * 1e-11);
            else
                lolunit_assert_doubles_equal(a, b, fabs(f) * 1e-11);
        }
    }
};

} /* namespace lol */

