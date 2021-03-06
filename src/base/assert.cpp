//
//  Lol Engine
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

#if HAVE_CXXABI_H
#   include <cxxabi.h>
#endif
#if HAVE_EXECINFO_H
#   include <execinfo.h>
#endif

#if EMSCRIPTEN
#   include <emscripten.h>
#endif

namespace lol
{

void DumpStack()
{
#if EMSCRIPTEN
    /* This would require demangling but we don't care yet. */
    msg::debug("%s\n", emscripten_run_script_string("(new Error).stack"));
#elif HAVE_CXA_DEMANGLE && HAVE_BACKTRACE_SYMBOLS
    /* Get current stack frames */
    void *stack_ptrs[50];
    size_t size = backtrace(stack_ptrs, 50);
    char **callstack = backtrace_symbols(stack_ptrs, size);

    if (size > 1)
        msg::debug("%d functions in stack trace:\n", (int)size - 1);

    /* Parse stack frames, skipping the first element (because
     * that’s ourselves) and print information. */
    for (size_t i = 1; i < size; ++i)
    {
        char *name = 0, *offset = 0, *address = 0;

        for (char *p = callstack[i]; *p; ++p)
        {
            if (*p == '(')
                name = p;
            else if (*p == '+')
                offset = p;
            else if (*p == ')')
            {
                address = p;
                break;
            }
        }

        if (name && offset && address && name < offset)
        {
            *name++ = *offset++ = *address++ = '\0';

            int ret;
            char *realname = abi::__cxa_demangle(name, 0, 0, &ret);

            if (ret == 0)
                name = realname;

            msg::debug("#%d %s: %s+%s %s\n", (int)i,
                       callstack[i], name, offset, address);
            free(realname);
        }
        else
        {
            msg::debug("#%d %s\n", (int)i, callstack[i]);
        }
    }

    free(callstack);
#endif
}

} /* namespace lol */

