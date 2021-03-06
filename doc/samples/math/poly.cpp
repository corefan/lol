//
//  Lol Engine — Sample math program: check trigonometric functions
//
//  Copyright © 2005—2011 Sam Hocevar <sam@hocevar.net>
//
//  This program is free software. It comes without any warranty, to
//  the extent permitted by applicable law. You can redistribute it
//  and/or modify it under the terms of the Do What the Fuck You Want
//  to Public License, Version 2, as published by the WTFPL Task Force.
//  See http://www.wtfpl.net/ for more details.
//

#if HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdio>

#include <lol/engine.h>

using namespace lol;

mat4 multiply_copy(mat4 m1, mat4 m2, mat4 m3)
{
    return m1 * m2 * m3;
}

mat4 multiply_ref(mat4 const &m1, mat4 const &m2, mat4 const &m3)
{
    return m1 * m2 * m3;
}

int main(int argc, char **argv)
{
    UNUSED(argc, argv);

    mat4 a = mat4::rotate(0.1f, vec3(1.f, 1.f, 0.f));
    mat4 b = mat4::rotate(0.1f, vec3(0.f, 0.f, 1.f));
    mat4 m = mat4(1.f);

    for (int i = 0; i < 40000000; ++i)
        //m = multiply_copy(a, m, b);
        m = multiply_ref(a, m, b);
    m.printf();

    return EXIT_SUCCESS;
}

