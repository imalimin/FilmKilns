/*
 * Copyright (c) 2018-present, lmyooyo@gmail.com.
 *
 * This source code is licensed under the GPL license found in the
 * LICENSE file in the root directory of this source tree.
 */

#ifndef HWVC_ANDROID_HWANDROIDGRAPHICBUFFER_H
#define HWVC_ANDROID_HWANDROIDGRAPHICBUFFER_H

#include "HwAbsGraphicBuffer.h"
#include "HwUILoader.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>

class HwAndroidGraphicBuffer : public HwAbsGraphicBuffer {
public:
    HwAndroidGraphicBuffer(int w, int h);

    ~HwAndroidGraphicBuffer();

    bool bind() override;

    bool read(uint8_t *pixels) override;

private:
    // Really I have no idea, but this should be big enough
    const int GRAPHIC_BUFFER_SIZE = 1024;
    uint8_t *handler = nullptr;
    EGLDisplay dpy = EGL_NO_DISPLAY;
    EGLImageKHR pImageKHR = nullptr;

public:
    enum class Format : int {
        HAL_PIXEL_FORMAT_RGBA_8888 = 1,
        HAL_PIXEL_FORMAT_RGBX_8888 = 2,
        HAL_PIXEL_FORMAT_RGB_888 = 3,
        HAL_PIXEL_FORMAT_RGB_565 = 4,
        HAL_PIXEL_FORMAT_BGRA_8888 = 5,
        HAL_PIXEL_FORMAT_RGBA_5551 = 6,
        HAL_PIXEL_FORMAT_RGBA_4444 = 7,
    };

    enum class Usage : int {
        /* buffer is never read in software */
                GRALLOC_USAGE_SW_READ_NEVER = 0x00000000,
        /* buffer is rarely read in software */
                GRALLOC_USAGE_SW_READ_RARELY = 0x00000002,
        /* buffer is often read in software */
                GRALLOC_USAGE_SW_READ_OFTEN = 0x00000003,
        /* mask for the software read values */
                GRALLOC_USAGE_SW_READ_MASK = 0x0000000F,

        /* buffer is never written in software */
                GRALLOC_USAGE_SW_WRITE_NEVER = 0x00000000,
        /* buffer is never written in software */
                GRALLOC_USAGE_SW_WRITE_RARELY = 0x00000020,
        /* buffer is never written in software */
                GRALLOC_USAGE_SW_WRITE_OFTEN = 0x00000030,
        /* mask for the software write values */
                GRALLOC_USAGE_SW_WRITE_MASK = 0x000000F0,

        /* buffer will be used as an OpenGL ES texture */
                GRALLOC_USAGE_HW_TEXTURE = 0x00000100,
        /* buffer will be used as an OpenGL ES render target */
                GRALLOC_USAGE_HW_RENDER = 0x00000200,
        /* buffer will be used by the 2D hardware blitter */
                GRALLOC_USAGE_HW_2D = 0x00000400,
        /* buffer will be used with the framebuffer device */
                GRALLOC_USAGE_HW_FB = 0x00001000,
        /* mask for the software usage bit-mask */
                GRALLOC_USAGE_HW_MASK = 0x00001F00,
    };
};


#endif //HWVC_ANDROID_HWANDROIDGRAPHICBUFFER_H
