//
// Lol Engine
//
// Copyright: (c) 2010-2011 Sam Hocevar <sam@hocevar.net>
//   This program is free software; you can redistribute it and/or
//   modify it under the terms of the Do What The Fuck You Want To
//   Public License, Version 2, as published by Sam Hocevar. See
//   http://sam.zoy.org/projects/COPYING.WTFPL for more details.
//

#if defined HAVE_CONFIG_H
#   include "config.h"
#endif

#include <cstdlib>
#include <cmath>

#ifdef WIN32
#   define WIN32_LEAN_AND_MEAN
#   include <windows.h>
#endif
#if defined __APPLE__ && defined __MACH__
#   include <OpenGL/gl.h>
#else
#   define GL_GLEXT_PROTOTYPES
#   include <GL/gl.h>
#endif

#include <SDL.h>
#include <SDL_image.h>

#include "core.h"

/*
 * TileSet implementation class
 */

class TileSetData
{
    friend class TileSet;

private:
    char *name;
    int *tiles;
    int w, h, nw, nh, ntiles;
    float dilate, tx, ty;

    SDL_Surface *img;
    GLuint texture;
};

/*
 * Public TileSet class
 */

TileSet::TileSet(char const *path, int w, int h, float dilate)
  : data(new TileSetData())
{
    data->name = strdup(path);
    data->tiles = NULL;
    data->img = NULL;
    data->texture = 0;

    for (char const *name = path; *name; name++)
        if ((data->img = IMG_Load(name)))
            break;

    if (!data->img)
    {
#if !FINAL_RELEASE
        fprintf(stderr, "ERROR: could not load %s\n", path);
#endif
        SDL_Quit();
        exit(1);
    }

    if (w <= 0)
        w = 32;
    if (h <= 0)
        h = 32;

    data->w = w;
    data->h = h;
    data->dilate = dilate;
    /* FIXME: check for non-zero here */
    data->nw = data->img->w / w;
    data->nh = data->img->h / h;
    data->ntiles = data->nw * data->nh;
    data->tx = (float)w / PotUp(data->img->w);
    data->ty = (float)h / PotUp(data->img->h);

    drawgroup = DRAWGROUP_BEFORE;
}

TileSet::~TileSet()
{
    free(data->tiles);
    free(data->name);
    delete data;
}

void TileSet::TickDraw(float deltams)
{
    Entity::TickDraw(deltams);

    if (destroy)
    {
        if (data->img)
            SDL_FreeSurface(data->img);
        else
            glDeleteTextures(1, &data->texture);
    }
    else if (data->img)
    {
        GLuint format = data->img->format->Amask ? GL_RGBA : GL_RGB;
        int planes = data->img->format->Amask ? 4 : 3;

        int w = PotUp(data->img->w);
        int h = PotUp(data->img->h);

        uint8_t *pixels = (uint8_t *)data->img->pixels;
        if (w != data->img->w || h != data->img->h)
        {
            uint8_t *tmp = (uint8_t *)malloc(planes * w * h);
            for (int line = 0; line < data->img->h; line++)
                memcpy(tmp + planes * w * line,
                       pixels + planes * data->img->w * line,
                       planes * data->img->w);
            pixels = tmp;
        }

        glGenTextures(1, &data->texture);
        glBindTexture(GL_TEXTURE_2D, data->texture);

        glTexImage2D(GL_TEXTURE_2D, 0, planes, w, h, 0,
                     format, GL_UNSIGNED_BYTE, pixels);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

        if (pixels != data->img->pixels)
            free(pixels);
        SDL_FreeSurface(data->img);
        data->img = NULL;
    }
}

char const *TileSet::GetName()
{
    return data->name;
}

void TileSet::BlitTile(uint32_t id, int x, int y, int z, int o)
{
    float tx = data->tx * ((id & 0xffff) % data->nw);
    float ty = data->ty * ((id & 0xffff) / data->nw);
    float dilate = data->dilate;

    int off = o ? data->h : 0;
    int dx = data->w;
    int dy = data->h * 38 / 32; /* Magic... fix this one day */
    int dy2 = data->h * 70 / 32;

    if (!data->img)
    {
        glBindTexture(GL_TEXTURE_2D, data->texture);
        glBegin(GL_QUADS);
            glTexCoord2f(tx, ty);
            glVertex3f(x, dilate * (y - dy - off), dilate * (z + off));
            glTexCoord2f(tx + data->tx, ty);
            glVertex3f(x + dx, dilate * (y - dy - off), dilate * (z + off));
            glTexCoord2f(tx + data->tx, ty + data->ty);
            glVertex3f(x + dx, dilate * (y - dy2), dilate * z);
            glTexCoord2f(tx, ty + data->ty);
            glVertex3f(x, dilate * (y - dy2), dilate * z);
        glEnd();
    }
}

