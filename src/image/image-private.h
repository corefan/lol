//
// Lol Engine
//
// Copyright: (c) 2010-2014 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The ImageCodecData class
// ------------------------
//

namespace lol
{

class PixelDataBase
{
public:
    virtual void *data() = 0;
    virtual void const *data() const = 0;
    virtual void *data2d() = 0;
    virtual void const *data2d() const = 0;

    inline virtual ~PixelDataBase() {}
};

template<PixelFormat T>
class PixelData : public PixelDataBase
{
public:
    inline PixelData(ivec2 size) { m_array2d.resize(size); }

    virtual void *data() { return m_array2d.data(); }
    virtual void const *data() const { return m_array2d.data(); }
    virtual void *data2d() { return &m_array2d; }
    virtual void const *data2d() const { return &m_array2d; }

    array2d<typename PixelType<T>::type> m_array2d;
};

class image_data
{
    friend class image;

public:
    image_data()
      : m_size(0, 0),
        m_wrap_x(WrapMode::Clamp),
        m_wrap_y(WrapMode::Clamp),
        m_format(PixelFormat::Unknown)
    {}

    ivec2 m_size;

    /* The wrap modes for pixel access */
    WrapMode m_wrap_x, m_wrap_y;

    /* A map of the various available bitplanes */
    map<int, PixelDataBase *> m_pixels;
    /* The last bitplane being accessed for writing */
    PixelFormat m_format;
};

} /* namespace lol */

