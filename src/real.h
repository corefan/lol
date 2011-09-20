//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

//
// The Real class
// --------------
//

#if !defined __LOL_REAL_H__
#define __LOL_REAL_H__

#include <stdint.h>

namespace lol
{

template<int NBITS> class Real
{
public:
    inline Real<NBITS>() {}
    Real<NBITS>(float f);

    operator float() const;

private:
    uint32_t m_signexp;
    uint32_t m_mantissa[(NBITS + 31) / 32];
};

typedef Real<4096> real4k;

} /* namespace lol */

#endif // __LOL_REAL_H__
