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

#if defined __ANDROID__

#include <jni.h>
#include <android/log.h>

extern "C" {
#include <android_native_app_glue.h>
}

#include "../../image/resource-private.h"

namespace lol
{

extern ANativeActivity *g_activity;

/*
 * Image implementation class
 */

class AndroidImageCodec : public ResourceCodec
{
public:
    virtual char const *GetName() { return "<AndroidImageCodec>"; }
    virtual ResourceCodecData* Load(char const *path);
    virtual bool Save(char const *path, ResourceCodecData* data);
    virtual bool Close();

    virtual uint8_t *GetData() const;

private:
    jobject m_bmp;
    jintArray m_array;
    jint *m_pixels;
};

DECLARE_IMAGE_CODEC(AndroidImageCodec, 100)

ResourceCodecData* AndroidImageCodec::Load(char const *path)
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, trying to attach thread\n");
#endif
        res = g_activity->vm->AttachCurrentThread(&env, nullptr);
    }
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, cannot open image %s\n", path);
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    mid = env->GetMethodID(cls, "openImage",
                           "(Ljava/lang/String;)Landroid/graphics/Bitmap;");
    jstring name = env->NewStringUTF(path);
    m_bmp = env->CallObjectMethod(g_activity->clazz, mid, name);
    env->DeleteLocalRef(name);
    if (!m_bmp)
    {
#if !LOL_BUILD_RELEASE
        msg::error("could not load %s\n", path);
#endif
        return false;
    }
    env->NewGlobalRef(m_bmp);

    /* Get image dimensions */
    mid = env->GetMethodID(cls, "getWidth", "(Landroid/graphics/Bitmap;)I");
    m_size.x = env->CallIntMethod(g_activity->clazz, mid, m_bmp);
    mid = env->GetMethodID(cls, "getHeight", "(Landroid/graphics/Bitmap;)I");
    m_size.y = env->CallIntMethod(g_activity->clazz, mid, m_bmp);

    /* Get pixels */
    m_array = env->NewIntArray(m_size.x * m_size.y);
    env->NewGlobalRef(m_array);
    mid = env->GetMethodID(cls, "getPixels", "(Landroid/graphics/Bitmap;[I)V");
    env->CallVoidMethod(g_activity->clazz, mid, m_bmp, m_array);

    m_pixels = env->GetIntArrayElements(m_array, 0);
    for (int n = 0; n < m_size.x * m_size.y; n++)
    {
        uint32_t u = m_pixels[n];
        u = (u & 0xff00ff00) | ((u & 0xff0000) >> 16) | ((u & 0xff) << 16);
        m_pixels[n] = u;
    }
    m_format = PixelFormat::RGBA_8;

    return new ResourceCodecData();
}

bool AndroidImageCodec::Save(char const *path, ResourceCodecData* data)
{
    UNUSED(path, data);

    /* TODO: unimplemented */
}

bool AndroidImageCodec::Close()
{
    JNIEnv *env;
    jint res = g_activity->vm->GetEnv((void **)&env, JNI_VERSION_1_2);
    if (res < 0)
    {
#if !LOL_BUILD_RELEASE
        msg::error("JVM environment not found, cannot close image\n");
#endif
        return false;
    }
    jclass cls = env->GetObjectClass(g_activity->clazz);
    jmethodID mid;

    env->ReleaseIntArrayElements(m_array, m_pixels, 0);
    env->DeleteGlobalRef(m_array);

    /* Free image */
    mid = env->GetMethodID(cls, "closeImage", "(Landroid/graphics/Bitmap;)V");
    env->CallVoidMethod(g_activity->clazz, mid, m_bmp);
    env->DeleteGlobalRef(m_bmp);

    return true;
}

uint8_t *AndroidImageCodec::GetData() const
{
    return (uint8_t *)m_pixels;
}

} /* namespace lol */

#endif /* __ANDROID__ */

