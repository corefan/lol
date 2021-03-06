//
// Lol Engine
//
// Copyright: (c) 2010-2013 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://www.wtfpl.net/ for more details.
//

#pragma once

//
// The TileSet class
// -----------------
// A TileSet is a collection of tiles stored in a texture. Texture uploading
// and freeing is done in the render tick method. When the refcount drops to
// zero, the texture is freed.
//

#include <lol/image/resource.h>
#include <lol/image/image.h>
#include <lol/gpu/texture.h>

#include <stdint.h>

#include "engine/entity.h"

namespace lol
{

class TextureImageData;

class TextureImage : public Entity
{
    typedef Entity super;
protected:
    virtual TextureImageData* GetNewData();

public:
    TextureImage(char const *path);
    TextureImage(char const *path, image* img);
    virtual ~TextureImage();

protected:
    void Init(char const *path);
    virtual void Init(char const *path, ResourceCodecData* loaded_data);
    virtual void Init(char const *path, image* img);

protected:
    virtual void TickDraw(float seconds, Scene &scene);

public:
    /* Inherited from Entity */
    virtual char const *GetName();

    void UpdateTexture(image* img);
    Texture * GetTexture();
    Texture const * GetTexture() const;
    image * GetImage();
    image const * GetImage() const;
    ivec2 GetImageSize() const;
    ivec2 GetTextureSize() const;
    void Bind();
    void Unbind();

protected:
    TextureImageData* m_data = nullptr;
};

} /* namespace lol */

