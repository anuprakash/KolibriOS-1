/*
 * Copyright (c) 2012 Intel Corporation. All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.
 * IN NO EVENT SHALL PRECISION INSIGHT AND/OR ITS SUPPLIERS BE LIABLE FOR
 * ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#ifdef IN_LIBVA
# include "va/drm/va_drm.h"
#else
# include <va/va_drm.h>
#endif
#include "va_display.h"

static int drm_fd = 0;

static VADisplay
va_open_display_drm(void)
{
    VADisplay va_dpy;

    drm_fd = get_service("DISPLAY");
    if (drm_fd == 0)
        return NULL;

    va_dpy = vaGetDisplayDRM(drm_fd);
    if (va_dpy)
        return va_dpy;

    drm_fd = 0;
    return NULL;
}

static void
va_close_display_drm(VADisplay va_dpy)
{
    drm_fd = 0;
}


static VAStatus
va_put_surface_drm(
    VADisplay          va_dpy,
    VASurfaceID        surface,
    const VARectangle *src_rect,
    const VARectangle *dst_rect
)
{
    return VA_STATUS_ERROR_OPERATION_FAILED;
}

const VADisplayHooks va_display_hooks_drm = {
    "drm",
    va_open_display_drm,
    va_close_display_drm,
    va_put_surface_drm,
};
