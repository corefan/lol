
include $(top_srcdir)/build/autotools/common.am

SUBDIRS = 3rdparty data . t

noinst_LIBRARIES = liblol-core.a

EXTRA_DIST += lol-core.vcxproj

liblol_core_a_SOURCES = \
    tiler.cpp tiler.h dict.cpp dict.h lolgl.h \
    scene.cpp scene.h font.cpp font.h \
    textureimage.cpp textureimage.h textureimage-private.h \
    tileset.cpp tileset.h forge.cpp forge.h video.cpp video.h \
    profiler.cpp profiler.h text.cpp text.h emitter.cpp emitter.h \
    numeric.h utils.h messageservice.cpp messageservice.h \
    gradient.cpp gradient.h gradient.lolfx \
    platform.cpp platform.h sprite.cpp sprite.h camera.cpp camera.h \
    light.cpp light.h \
    \
    $(liblol_core_headers) \
    $(liblol_core_sources) \
    \
    $(imgui_sources) \
    $(nacl_sources) \
    $(sdl_sources) \
    $(d3d9_sources) \
    $(android_sources)
#liblol_core_a_CPPFLAGS = -include-pch $(srcdir)/lol/engine-internal.h.pch $(AM_CPPFLAGS) -I$(srcdir)/3rdparty/bullet3/src -I$(srcdir)/3rdparty/lua/src
liblol_core_a_CPPFLAGS = $(AM_CPPFLAGS) -I$(srcdir)/3rdparty/bullet3/src -I$(srcdir)/3rdparty/lua/src

liblol_core_headers = \
    lol/engine-internal.h lol/engine.h lol/public.h lol/extras.h lol/legacy.h \
    \
    lol/base/all.h \
    lol/base/avl_tree.h lol/base/features.h lol/base/tuple.h lol/base/types.h \
    lol/base/array.h lol/base/assert.h lol/base/string.h lol/base/hash.h \
    lol/base/map.h lol/base/enum.h lol/base/log.h \
    \
    lol/math/all.h \
    lol/math/functions.h lol/math/vector.h lol/math/half.h lol/math/real.h \
    lol/math/geometry.h lol/math/interp.h lol/math/rand.h lol/math/arraynd.h \
    lol/math/constants.h lol/math/matrix.h lol/math/ops.h \
    lol/math/transform.h lol/math/polynomial.h lol/math/bigint.h \
    lol/math/noise/gradient.h lol/math/noise/perlin.h \
    lol/math/noise/simplex.h \
    \
    lol/algorithm/all.h \
    lol/algorithm/sort.h lol/algorithm/portal.h lol/algorithm/aabb_tree.h \
    \
    lol/audio/all.h \
    lol/audio/audio.h lol/audio/sampler.h lol/audio/sample.h \
    \
    lol/sys/all.h \
    lol/sys/init.h lol/sys/file.h lol/sys/getopt.h lol/sys/thread.h \
    lol/sys/threadtypes.h lol/sys/timer.h \
    \
    lol/image/all.h \
    lol/image/pixel.h lol/image/color.h lol/image/image.h lol/image/resource.h lol/image/movie.h \
    \
    lol/gpu/all.h \
    lol/gpu/shader.h lol/gpu/indexbuffer.h lol/gpu/vertexbuffer.h \
    lol/gpu/framebuffer.h lol/gpu/texture.h lol/gpu/lolfx.h \
    lol/gpu/renderer.h lol/gpu/rendercontext.h \
    \
    lol/debug/all.h \
    lol/debug/lines.h

liblol_core_sources = \
    application/application.cpp application/application.h \
    \
    lolua/baselua.cpp lolua/baselua.h \
    \
    eglapp.cpp eglapp.h \
    \
    commandstack.h \
    easymesh/easymeshbuild.cpp easymesh/easymeshbuild.h \
    easymesh/easymeshrender.cpp easymesh/easymeshrender.h \
    easymesh/easymesh.cpp \
    easymesh/easymeshinternal.cpp easymesh/easymeshcsg.cpp \
    easymesh/easymeshprimitive.cpp easymesh/easymeshtransform.cpp \
    easymesh/easymeshcursor.cpp easymesh/easymesh.h \
    easymesh/easymeshlua.cpp easymesh/easymeshlua.h \
    easymesh/csgbsp.cpp easymesh/csgbsp.h \
    easymesh/shiny.lolfx easymesh/shinyflat.lolfx \
    easymesh/shinydebugwireframe.lolfx \
    easymesh/shinydebuglighting.lolfx easymesh/shinydebugnormal.lolfx \
    easymesh/shinydebugUV.lolfx easymesh/shiny_SK.lolfx \
    \
    base/assert.cpp base/hash.cpp base/log.cpp base/string.cpp \
    base/enum.cpp \
    \
    math/vector.cpp math/matrix.cpp math/transform.cpp math/trig.cpp \
    math/constants.cpp math/geometry.cpp math/real.cpp math/half.cpp \
    \
    gpu/shader.cpp gpu/indexbuffer.cpp gpu/vertexbuffer.cpp \
    gpu/framebuffer.cpp gpu/texture.cpp gpu/renderer.cpp \
    gpu/rendercontext.cpp \
    \
    audio/audio.cpp audio/sampler.cpp audio/sample.cpp \
    \
    input/input.cpp input/input.h input/input_internal.h input/keys.h \
    input/controller.cpp input/controller.h \
    \
    gpu/default-material.lolfx \
    gpu/empty-material.lolfx \
    gpu/test-material.lolfx \
    gpu/tile.lolfx gpu/palette.lolfx gpu/line.lolfx \
    gpu/blit.lolfx \
    gpu/postprocess.lolfx \
    \
    gpu/lolfx.cpp \
    \
    mesh/mesh.cpp mesh/mesh.h \
    mesh/primitivemesh.cpp mesh/primitivemesh.h \
    \
    sys/init.cpp sys/file.cpp sys/hacks.cpp \
    sys/thread.cpp sys/threadtypes.cpp sys/getopt.cpp \
    \
	image/resource.cpp image/resource-private.h \
    image/image.cpp image/image-private.h image/kernel.cpp image/pixel.cpp \
    image/crop.cpp image/resample.cpp image/noise.cpp image/combine.cpp \
    image/codec/gdiplus-image.cpp image/codec/imlib2-image.cpp \
    image/codec/sdl-image.cpp image/codec/ios-image.cpp \
    image/codec/zed-image.cpp image/codec/zed-palette-image.cpp \
    image/codec/oric-image.cpp image/codec/dummy-image.cpp \
    image/color/cie1931.cpp image/color/color.cpp \
    image/dither/random.cpp image/dither/ediff.cpp image/dither/dbs.cpp \
    image/dither/ostromoukhov.cpp image/dither/ordered.cpp \
    image/filter/convolution.cpp image/filter/colors.cpp \
    image/filter/dilate.cpp image/filter/median.cpp image/filter/yuv.cpp \
    image/movie.cpp \
    \
    engine/ticker.cpp engine/ticker.h \
    engine/entity.cpp engine/entity.h \
    engine/world.cpp engine/world.h \
    engine/worldentity.cpp engine/worldentity.h \
    \
    loldebug.h \
    debug/fps.cpp debug/fps.h debug/lines.cpp \
    debug/record.cpp debug/record.h debug/stats.cpp debug/stats.h

sdl_sources = \
    platform/sdl/sdlapp.cpp platform/sdl/sdlapp.h \
    platform/sdl/sdlinput.cpp platform/sdl/sdlinput.h

d3d9_sources = \
    platform/d3d9/d3d9input.cpp platform/d3d9/d3d9input.h

imgui_sources = \
    lolimgui.cpp lolimgui.h \
    3rdparty/imgui/imgui_demo.cpp \
    3rdparty/imgui/imgui_draw.cpp \
    $(NULL)

if LOL_USE_NACL
nacl_sources = \
    platform/nacl/nacl-app.cpp platform/nacl/nacl-app.h \
    platform/nacl/nacl-instance.cpp platform/nacl/nacl-instance.h \
    platform/nacl/nacl-module.cpp \
    platform/nacl/opengl_context.cpp platform/nacl/opengl_context.h \
    platform/nacl/opengl_context_ptrs.h
endif

android_sources = \
    image/codec/android-image.cpp \
    platform/android/androidapp.cpp platform/android/androidapp.h

