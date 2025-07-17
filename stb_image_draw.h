// stb_image_draw.h - v0.1 - simple draw to image
// Sean Barrett, Fri Jul 26 2024
// author dianjixz

#if 0
// SAMPLE CODE:
//
//    Here's sample code for old OpenGL; it's a lot more complicated
//    to make work on modern APIs, and that's your problem.
//
#endif

#ifndef INCLUDE_STB_IMAGE_DRAW_H
#define INCLUDE_STB_IMAGE_DRAW_H


#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>



// if STB_IMAGE_WRITE_STATIC causes problems, try defining STBIWDEF to 'inline' or 'static inline'
#ifndef STBIWDEF
#ifdef STB_IMAGE_WRITE_STATIC
#define STBIWDEF  static
#else
#ifdef __cplusplus
#define STBIWDEF  extern "C"
#else
#define STBIWDEF  extern
#endif
#endif
#endif


#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
// ARGB
#else
//cpu is little
#endif //__BYTE_ORDER__

#define COLOR_R8_G8_B8_TO_RGB565(r8, g8, b8) (uint16_t)((((uint16_t)(r8)&(uint16_t)0xf8) << 8)|(((uint16_t)(g8)&(uint16_t)0xfc)<<3)|(((uint16_t)(b8)&(uint16_t)0xf8)>>3))
#define COLOR_R8_G8_B8_TO_RGB888(r, g, b) (uint32_t)(((uint32_t)(r)<<16)|((uint32_t)(g)<<8)|((uint32_t)(b)))
#define COLOR_R8_G8_B8_TO_BGR888(r, g, b) (uint32_t)(((uint32_t)(b)<<16)|((uint32_t)(g)<<8)|((uint32_t)(r)))

#ifndef M_PI
#define M_PI    3.14159265f
#endif
#ifndef M_PI_2
#define M_PI_2  1.57079632f
#endif
#ifndef M_PI_4
#define M_PI_4  0.78539816f
#endif

#define IM_MAX(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define IM_MIN(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define IM_DIV(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a / _b) : 0; })
#define IM_MOD(a,b)     ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _b ? (_a % _b) : 0; })
#define IM_DEG2RAD(x)   (((x)*M_PI)/180)
#define IM_RAD2DEG(x)   (((x)*180)/M_PI)

STBIWDEF inline uint32_t stb_image_draw_get_pixel(void *data, uint32_t stride, int x, int y, uint32_t comp);
STBIWDEF inline void stb_image_draw_set_pixel(void *data, uint32_t stride, int x, int y, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_draw_point_fill(void  *data, uint32_t stride, int cx, int cy, int r0, int r1, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_draw_line(void  *data, uint32_t stride, int x0, int y0, int x1, int y1, uint32_t c, uint32_t comp, int thickness);
STBIWDEF void stb_image_draw_xLine(void  *data, uint32_t stride, int x1, int x2, int y, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_draw_yLine(void  *data, uint32_t stride, int x, int y1, int y2, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_draw_rectangle(void  *data, uint32_t stride, int rx, int ry, int rw, int rh, uint32_t c, uint32_t comp, int thickness, bool fill);
STBIWDEF void stb_image_draw_draw_circle(void  *data, uint32_t stride, int cx, int cy, int r, uint32_t c, uint32_t comp, int thickness, bool fill);
STBIWDEF void stb_image_draw_cross(void  *data, uint32_t stride, int x, int y, uint32_t c, uint32_t comp, int size, int thickness);
STBIWDEF void stb_image_scratch_draw_pixel(void  *data, uint32_t stride, int x0, int y0, int dx, int dy, float shear_dx, float shear_dy, int r0, int r1, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_scratch_draw_line(void  *data, uint32_t stride, int x0, int y0, int dx, int dy0, int dy1, float shear_dx, float shear_dy, uint32_t c, uint32_t comp);
STBIWDEF void stb_image_scratch_draw_sheared_ellipse(void  *data, uint32_t stride, int x0, int y0, int width, int height, bool filled, float shear_dx, float shear_dy, uint32_t c, uint32_t comp, int thickness);
STBIWDEF void stb_image_scratch_draw_rotated_ellipse(void  *data, uint32_t stride, int x, int y, int x_axis, int y_axis, int rotation, bool filled, uint32_t c, uint32_t comp, int thickness);
STBIWDEF void stb_image_draw_ellipse(void  *data, uint32_t stride, int cx, int cy, int rx, int ry, int rotation, uint32_t c, uint32_t comp, int thickness, bool fill);
STBIWDEF void stb_image_draw_image(void  *dist, uint32_t dstride, void  *src, uint32_t sstride, int dx, int dy, int sw, int sh, unsigned char alpha);




#ifdef STB_IMAGE_DRAW_IMPLEMENTATION

STBIWDEF inline void stb_image_draw_set_pixel(void *data, uint32_t stride, int x, int y, uint32_t c, uint32_t comp)
{
    uint8_t *ptr = (uint8_t *)data + stride * y + comp * x;
    uint8_t *ptrc = (uint8_t *)&c;
    switch (comp) {
        case 4:ptr[3] = ptrc[3];
        case 3:ptr[2] = ptrc[2];
        case 2:ptr[1] = ptrc[1];
        case 1:ptr[0] = ptrc[0];
        default:
            break;
    }
}

STBIWDEF inline uint32_t stb_image_draw_get_pixel(void *data, uint32_t stride, int x, int y, uint32_t comp)
{
    uint32_t c = 0;
    uint8_t* ptr = (uint8_t*)data + stride * y + comp * x;
    uint8_t *ptrc = &c;
    switch (comp) {
        case 4:ptrc[3] = ptr[3];
        case 3:ptrc[2] = ptr[2];
        case 2:ptrc[1] = ptr[1];
        case 1:ptrc[0] = ptr[0];
        default:
            break;
    }
    return c;
}

STBIWDEF void stb_image_draw_point_fill(const void  *data, uint32_t stride, int cx, int cy, int r0, int r1, uint32_t c, uint32_t comp)
{
    for (int y = r0; y <= r1; y++) {
        for (int x = r0; x <= r1; x++) {
            if (((x * x) + (y * y)) <= (r0 * r0)) {
                stb_image_draw_set_pixel(data, stride, cx + x, cy + y, c, comp);
            }
        }
    }
}

STBIWDEF void stb_image_draw_line(void  *data, uint32_t stride, int x0, int y0, int x1, int y1, uint32_t c, uint32_t comp, int thickness)
{
    if (thickness == 1) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = ((dx > dy) ? dx : -dy) / 2;

        for (;;) {
            stb_image_draw_set_pixel(data, stride, x0, y0, c, comp);
            if ((x0 == x1) && (y0 == y1)) break;
            int e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 <  dy) { err += dx; y0 += sy; }
        }
    }
    else {
        int thickness0 = (thickness - 0) / 2;
        int thickness1 = (thickness - 1) / 2;
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = ((dx > dy) ? dx : -dy) / 2;

        for (;;) {
            stb_image_draw_point_fill(data, stride, x0, y0, -thickness0, thickness1, c, comp);
            if ((x0 == x1) && (y0 == y1)) break;
            int e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 <  dy) { err += dx; y0 += sy; }
        }
    }
}

STBIWDEF void stb_image_draw_xLine(void  *data, uint32_t stride, int x1, int x2, int y, uint32_t c, uint32_t comp)
{
    while (x1 <= x2) stb_image_draw_set_pixel(data, stride, x1++, y, c, comp);
}

STBIWDEF void stb_image_draw_yLine(void  *data, uint32_t stride, int x, int y1, int y2, uint32_t c, uint32_t comp)
{
    while (y1 <= y2) stb_image_draw_set_pixel(data, stride, x, y1++, c, comp);
}

STBIWDEF void stb_image_draw_rectangle(void  *data, uint32_t stride, int rx, int ry, int rw, int rh, uint32_t c, uint32_t comp, int thickness, bool fill)
{
    if (fill) {
        for (int y = ry, yy = ry + rh; y < yy; y++) {
            for (int x = rx, xx = rx + rw; x < xx; x++) {
                stb_image_draw_set_pixel(data, stride, x, y, c, comp);
            }
        }
    } else if (thickness > 0) {
        int thickness0 = (thickness - 0) / 2;
        int thickness1 = (thickness - 1) / 2;

        for (int i = rx - thickness0, j = rx + rw + thickness1, k = ry + rh - 1; i < j; i++) {
            stb_image_draw_yLine(data, stride, i, ry - thickness0, ry + thickness1, c, comp);
            stb_image_draw_yLine(data, stride, i, k - thickness0, k + thickness1, c, comp);
        }

        for (int i = ry - thickness0, j = ry + rh + thickness1, k = rx + rw - 1; i < j; i++) {
            stb_image_draw_xLine(data, stride, rx - thickness0, rx + thickness1, i, c, comp);
            stb_image_draw_xLine(data, stride, k - thickness0, k + thickness1, i, c, comp);
        }
    }
}
STBIWDEF void stb_image_draw_draw_circle(void  *data, uint32_t stride, int cx, int cy, int r, uint32_t c, uint32_t comp, int thickness, bool fill)
{
    if (fill) {
        stb_image_draw_point_fill(data, stride, cx, cy, -r, r, c, comp);
    } else if (thickness > 0) {
        int thickness0 = (thickness - 0) / 2;
        int thickness1 = (thickness - 1) / 2;

        int xo = r + thickness0;
        int xi = r > thickness1 ? r - thickness1 : 0;
        int xi_tmp = xi;
        int y = 0;
        int erro = 1 - xo;
        int erri = 1 - xi;

        while(xo >= y) {
            stb_image_draw_xLine(data, stride, cx + xi, cx + xo, cy + y,  c, comp);
            stb_image_draw_yLine(data, stride, cx + y,  cy + xi, cy + xo, c, comp);
            stb_image_draw_xLine(data, stride, cx - xo, cx - xi, cy + y,  c, comp);
            stb_image_draw_yLine(data, stride, cx - y,  cy + xi, cy + xo, c, comp);
            stb_image_draw_xLine(data, stride, cx - xo, cx - xi, cy - y,  c, comp);
            stb_image_draw_yLine(data, stride, cx - y,  cy - xo, cy - xi, c, comp);
            stb_image_draw_xLine(data, stride, cx + xi, cx + xo, cy - y,  c, comp);
            stb_image_draw_yLine(data, stride, cx + y,  cy - xo, cy - xi, c, comp);

            y++;

            if (erro < 0) {
                erro += 2 * y + 1;
            } else {
                xo--;
                erro += 2 * (y - xo + 1);
            }

            if (y > xi_tmp) {
                xi = y;
            } else {
                if (erri < 0) {
                    erri += 2 * y + 1;
                } else {
                    xi--;
                    erri += 2 * (y - xi + 1);
                }
            }
        }
    }
}

STBIWDEF void stb_image_draw_cross(void  *data, uint32_t stride, int x, int y, uint32_t c, uint32_t comp, int size, int thickness)
{
    stb_image_draw_line(data, stride, x - size, y        , x + size, y        , c, comp, thickness);
    stb_image_draw_line(data, stride, x        , y - size, x        , y + size, c, comp, thickness);
}

STBIWDEF void stb_image_scratch_draw_pixel(void  *data, uint32_t stride, int x0, int y0, int dx, int dy, float shear_dx, float shear_dy, int r0, int r1, uint32_t c, uint32_t comp)
{
    int _roundf = (int)((float)(((float)dx * (float)shear_dy) / (float)shear_dx) + 0.5f);
    stb_image_draw_point_fill(data, stride, x0 + dx, y0 + dy + _roundf, r0, r1, c, comp);
}

STBIWDEF void stb_image_scratch_draw_line(void  *data, uint32_t stride, int x0, int y0, int dx, int dy0, int dy1, float shear_dx, float shear_dy, uint32_t c, uint32_t comp)
{
    int y = y0 +  (int)((float)(((float)dx * (float)shear_dy) / (float)shear_dx) + 0.5f);
    stb_image_draw_yLine(data, stride, x0 + dx, y + dy0, y + dy1, c, comp);
}

STBIWDEF void stb_image_scratch_draw_sheared_ellipse(void  *data, uint32_t stride, int x0, int y0, int width, int height, bool filled, float shear_dx, float shear_dy, uint32_t c, uint32_t comp, int thickness)
{
    int thickness0 = (thickness - 0) / 2;
    int thickness1 = (thickness - 1) / 2;
    if (((thickness > 0) || filled) && (shear_dx != 0)) {
        int a_squared = width * width;
        int four_a_squared = a_squared * 4;
        int b_squared = height * height;
        int four_b_squared = b_squared * 4;

        int x = 0;
        int y = height;
        int sigma = (2 * b_squared) + (a_squared * (1 - (2 * height)));

        while ((b_squared * x) <= (a_squared * y)) {
            if (filled) {
                stb_image_scratch_draw_line(data, stride, x0, y0, x, -y, y, shear_dx, shear_dy, c, comp);
                stb_image_scratch_draw_line(data, stride, x0, y0, -x, -y, y, shear_dx, shear_dy, c, comp);
            } else {
                stb_image_scratch_draw_pixel(data, stride, x0, y0, x, y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, -x, y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, x, -y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, -x, -y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
            }

            if (sigma >= 0) {
                sigma += four_a_squared * (1 - y);
                y -= 1;
            }

            sigma += b_squared * ((4 * x) + 6);
            x += 1;
        }

        x = width;
        y = 0;
        sigma = (2 * a_squared) + (b_squared * (1 - (2 * width)));

        while ((a_squared * y) <= (b_squared * x)) {
            if (filled) {
                stb_image_scratch_draw_line(data, stride, x0, y0, x, -y, y, shear_dx, shear_dy, c, comp);
                stb_image_scratch_draw_line(data, stride, x0, y0, -x, -y, y, shear_dx, shear_dy, c, comp);
            } else {
                stb_image_scratch_draw_pixel(data, stride, x0, y0, x, y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, -x, y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, x, -y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
                stb_image_scratch_draw_pixel(data, stride, x0, y0, -x, -y, shear_dx, shear_dy, -thickness0, thickness1, c, comp);
            }

            if (sigma >= 0) {
                sigma += four_b_squared * (1 - x);
                x -= 1;
            }

            sigma += a_squared * ((4 * y) + 6);
            y += 1;
        }
    }
}

STBIWDEF void stb_image_scratch_draw_rotated_ellipse(void  *data, uint32_t stride, int x, int y, int x_axis, int y_axis, int rotation, bool filled, uint32_t c, uint32_t comp, int thickness)
{
    if ((x_axis > 0) && (y_axis > 0)) {
        if ((x_axis == y_axis) || (rotation == 0)) {
            stb_image_scratch_draw_sheared_ellipse(data, stride, x, y, x_axis / 2, y_axis / 2, filled, 1, 0, c, comp, thickness);
        } else if (rotation == 90) {
            stb_image_scratch_draw_sheared_ellipse(data, stride, x, y, y_axis / 2, x_axis / 2, filled, 1, 0, c, comp, thickness);
        } else {

            // Avoid rotations above 90.
            if (rotation > 90) {
                rotation -= 90;
                int temp = x_axis;
                x_axis = y_axis;
                y_axis = temp;
            }

            // Avoid rotations above 45.
            if (rotation > 45) {
                rotation -= 90;
                int temp = x_axis;
                x_axis = y_axis;
                y_axis = temp;
            }

            float theta = atanf(IM_DIV(y_axis, x_axis) * (-tanf(IM_DEG2RAD(rotation))));
            float shear_dx = (x_axis * cosf(theta) * cosf(IM_DEG2RAD(rotation))) - (y_axis * sinf(theta) * sinf(IM_DEG2RAD(rotation)));
            float shear_dy = (x_axis * cosf(theta) * sinf(IM_DEG2RAD(rotation))) + (y_axis * sinf(theta) * cosf(IM_DEG2RAD(rotation)));
            float shear_x_axis = fabsf(shear_dx);
            float shear_y_axis = IM_DIV((y_axis * x_axis), shear_x_axis);
            stb_image_scratch_draw_sheared_ellipse(data, stride, x, y, (int)floorf(shear_x_axis / 2), (int)floorf(shear_y_axis / 2), filled, shear_dx, shear_dy, c, comp, thickness);
        }
    }
}

STBIWDEF void stb_image_draw_ellipse(void  *data, uint32_t stride, int cx, int cy, int rx, int ry, int rotation, uint32_t c, uint32_t comp, int thickness, bool fill)
{
    int r = rotation % 180;
    if (r < 0) r += 180;

    stb_image_scratch_draw_rotated_ellipse(data, stride, cx, cy, rx * 2, ry * 2, r, fill, c, comp, thickness);
}
// static void stb_image_draw_flood_fill(void  *data, int x, int y,
//                       float seed_threshold, float floating_threshold,
//                       int c, bool invert, bool clear_background, void *mask);

// void imlib_draw_string(void *data, int x_off, int y_off, const char *str, int c, float scale, int x_spacing, int y_spacing, bool mono_space,
//                        int char_rotation, bool char_hmirror, bool char_vflip, int string_rotation, bool string_hmirror, bool string_hflip);
// void imlib_draw_image_fast(void *data, void *other, int x_off, int y_off, float x_scale, float y_scale, float alpha, void *mask);
#endif
#endif



/*
------------------------------------------------------------------------------
This software is available under 2 licenses -- choose whichever you prefer.
------------------------------------------------------------------------------
ALTERNATIVE A - MIT License
Copyright (c) 2017 Sean Barrett
Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
------------------------------------------------------------------------------
ALTERNATIVE B - Public Domain (www.unlicense.org)
This is free and unencumbered software released into the public domain.
Anyone is free to copy, modify, publish, use, compile, sell, or distribute this
software, either in source code form or as a compiled binary, for any purpose,
commercial or non-commercial, and by any means.
In jurisdictions that recognize copyright laws, the author or authors of this
software dedicate any and all copyright interest in the software to the public
domain. We make this dedication for the benefit of the public at large and to
the detriment of our heirs and successors. We intend this dedication to be an
overt act of relinquishment in perpetuity of all present and future rights to
this software under copyright law.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
------------------------------------------------------------------------------
*/
