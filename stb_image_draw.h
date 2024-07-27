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

// Pixel format IDs.
typedef enum {
    PIXFORMAT_ID_BINARY     = 1,
    PIXFORMAT_ID_GRAY       = 2,
    PIXFORMAT_ID_RGB565     = 3,
    PIXFORMAT_ID_BAYER      = 4,
    PIXFORMAT_ID_YUV422     = 5,
    PIXFORMAT_ID_BGR888     = 6,
    PIXFORMAT_ID_RGB888     = 7,
    PIXFORMAT_ID_ARGB8      = 8,
    PIXFORMAT_ID_RGBA8      = 9,
    PIXFORMAT_ID_BGRA8      = 10,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} stb_pixformat_id_t;

// Pixel sub-format IDs.
typedef enum {
    SUBFORMAT_ID_GRAY8      = 0,
    SUBFORMAT_ID_GRAY16     = 1,
    SUBFORMAT_ID_BGGR       = 0, // !!! Note: Make sure bayer sub-formats don't  !!!
    SUBFORMAT_ID_GBRG       = 1, // !!! overflow the sensor.hw_flags.bayer field !!!
    SUBFORMAT_ID_GRBG       = 2,
    SUBFORMAT_ID_RGGB       = 3,
    SUBFORMAT_ID_YUV422     = 0,
    SUBFORMAT_ID_YVU422     = 1,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} stb_subformat_id_t;

// Pixel format Byte Per Pixel.
typedef enum {
    PIXFORMAT_BPP_BINARY    = 0,
    PIXFORMAT_BPP_GRAY8     = 1,
    PIXFORMAT_BPP_GRAY16    = 2,
    PIXFORMAT_BPP_RGB565    = 2,
    PIXFORMAT_BPP_BAYER     = 1,
    PIXFORMAT_BPP_YUV422    = 2,
    PIXFORMAT_BPP_888       = 3,
    PIXFORMAT_BPP_8888      = 4,
    /* Note: Update PIXFORMAT_IS_VALID when adding new formats */
} stb_pixformat_bpp_t;

// Pixel format flags.
#define PIXFORMAT_FLAGS_Y       (1 << 28) // YUV format.
#define PIXFORMAT_FLAGS_M       (1 << 27) // Mutable format.
#define PIXFORMAT_FLAGS_C       (1 << 26) // Colored format.
#define PIXFORMAT_FLAGS_J       (1 << 25) // Compressed format (JPEG/PNG).
#define PIXFORMAT_FLAGS_R       (1 << 24) // RAW/Bayer format.
#define PIXFORMAT_FLAGS_CY      (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_Y)
#define PIXFORMAT_FLAGS_CM      (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_M)
#define PIXFORMAT_FLAGS_CR      (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_R)
#define PIXFORMAT_FLAGS_CJ      (PIXFORMAT_FLAGS_C | PIXFORMAT_FLAGS_J)
#define IMLIB_IMAGE_MAX_SIZE(x) ((x) & 0xFFFFFFFF)

// Each pixel format encodes flags, pixel format id and bpp as follows:
// 31......29  28  27  26  25  24  23..........16  15...........8  7.............0
// <RESERVED>  YF  MF  CF  JF  RF  <PIXFORMAT_ID>  <SUBFORMAT_ID>  <BYTES_PER_PIX>
// NOTE: Bit 31-30 must Not be used for pixformat_t to be used as mp_int_t.
typedef enum {
    PIXFORMAT_INVALID    = (0x00000000U),
    PIXFORMAT_BINARY     = (PIXFORMAT_FLAGS_M  | (PIXFORMAT_ID_BINARY << 16) | (0                   << 8) | PIXFORMAT_BPP_BINARY ),
    PIXFORMAT_GRAYSCALE  = (PIXFORMAT_FLAGS_M  | (PIXFORMAT_ID_GRAY   << 16) | (SUBFORMAT_ID_GRAY8  << 8) | PIXFORMAT_BPP_GRAY8  ),
    PIXFORMAT_RGB565     = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_RGB565 << 16) | (0                   << 8) | PIXFORMAT_BPP_RGB565 ),
    PIXFORMAT_ARGB8      = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_ARGB8  << 16) | (0                   << 8) | PIXFORMAT_BPP_8888  ),
    PIXFORMAT_BAYER      = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_BGGR   << 8) | PIXFORMAT_BPP_BAYER  ),
    PIXFORMAT_BAYER_BGGR = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_BGGR   << 8) | PIXFORMAT_BPP_BAYER  ),
    PIXFORMAT_BAYER_GBRG = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_GBRG   << 8) | PIXFORMAT_BPP_BAYER  ),
    PIXFORMAT_BAYER_GRBG = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_GRBG   << 8) | PIXFORMAT_BPP_BAYER  ),
    PIXFORMAT_BAYER_RGGB = (PIXFORMAT_FLAGS_CR | (PIXFORMAT_ID_BAYER  << 16) | (SUBFORMAT_ID_RGGB   << 8) | PIXFORMAT_BPP_BAYER  ),
    PIXFORMAT_YUV        = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YUV422 << 8) | PIXFORMAT_BPP_YUV422 ),
    PIXFORMAT_YUV422     = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YUV422 << 8) | PIXFORMAT_BPP_YUV422 ),
    PIXFORMAT_YVU422     = (PIXFORMAT_FLAGS_CY | (PIXFORMAT_ID_YUV422 << 16) | (SUBFORMAT_ID_YVU422 << 8) | PIXFORMAT_BPP_YUV422 ),
    PIXFORMAT_RGB888     = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_RGB888 << 16) | (0                   << 8) | PIXFORMAT_BPP_888 ),
    PIXFORMAT_BGR888     = (PIXFORMAT_FLAGS_CM | (PIXFORMAT_ID_BGR888 << 16) | (0                   << 8) | PIXFORMAT_BPP_888 ),
    PIXFORMAT_LAST       = (0xFFFFFFFFU),
} stb_pixformat_t;


#if __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
// ARGB
#else
//cpu is little
#endif //__BYTE_ORDER__

typedef struct stb_image_s {
    void *rawdata;
    unsigned int w;
    unsigned int h;
    stb_pixformat_t format;
} stb_image_t;


#define COLOR_R8_G8_B8_TO_RGB565(r8, g8, b8) (uint16_t)((((uint16_t)(r8)&(uint16_t)0xf8) << 8)|(((uint16_t)(g8)&(uint16_t)0xfc)<<3)|(((uint16_t)(b8)&(uint16_t)0xf8)>>3))
#define COLOR_R8_G8_B8_TO_RGB888(r, g, b) (uint32_t)(((uint32_t)(r)<<16)|((uint32_t)(g)<<8)|((uint32_t)(b)))
#define COLOR_R8_G8_B8_TO_BGR888(r, g, b) (uint32_t)(((uint32_t)(b)<<16)|((uint32_t)(g)<<8)|((uint32_t)(r)))
#define COLOR_RGB888_TO_RGB565(c32) (uint16_t)((uint16_t)(((uint32_t)(c32)&(uint32_t)0xf80000)>>8)|(uint16_t)(((uint32_t)(c32)&(uint32_t)0xfc00)>>5)|(uint16_t)(((uint32_t)(c32)&(uint32_t)0xf8)>>3))
#define COLOR_BGR888_TO_RGB565(c32) (uint16_t)((uint16_t)(((uint32_t)(c32)&(uint32_t)0xf80000)>>19)|(uint16_t)(((uint32_t)(c32)&(uint32_t)0xfc00)>>5)|(uint16_t)(((uint32_t)(c32)&(uint32_t)0xf8)<<8))
#define COLOR_RGB565_TO_RGB888(c16) (uint32_t)((uint32_t)(((uint32_t)(c16)&(uint32_t)0xf800)<<8)|(uint32_t)(((uint32_t)(c16)&(uint32_t)0x7e0)<<3)|(uint32_t)(((uint32_t)(c16)&(uint32_t)0x1f)<<3))
#define COLOR_RGB565_TO_BGR888(c16) (uint32_t)((uint32_t)(((uint32_t)(c16)&(uint32_t)0xf800)>>8)|(uint32_t)(((uint32_t)(c16)&(uint32_t)0x7e0)<<3)|(uint32_t)(((uint32_t)(c16)&(uint32_t)0x1f)<<19))
#define COLOR_RGB888_TO_R8(c32) (uint8_t)(((uint32_t)(c32)>>16)&(uint32_t)0xff)
#define COLOR_RGB888_TO_G8(c32) (uint8_t)(((uint32_t)(c32)>>8)&(uint32_t)0xff)
#define COLOR_RGB888_TO_B8(c32) (uint8_t)((uint32_t)(c32)&(uint32_t)0xff)
#define COLOR_BGR888_TO_R8(c32) (uint8_t)((uint32_t)(c32)&(uint32_t)0xff)
#define COLOR_BGR888_TO_G8(c32) (uint8_t)(((uint32_t)(c32)>>8)&(uint32_t)0xff)
#define COLOR_BGR888_TO_B8(c32) (uint8_t)(((uint32_t)(c32)>>16)&(uint32_t)0xff)
#define COLOR_RGB565_TO_R8(c16) (uint8_t)((uint16_t)((uint16_t)(c16)&(uint16_t)0xf800)>>8)
#define COLOR_RGB565_TO_G8(c16) (uint8_t)((uint16_t)((uint16_t)(c16)&(uint16_t)0x07e0)>>3)
#define COLOR_RGB565_TO_B8(c16) (uint8_t)((uint16_t)((uint16_t)(c16)&(uint16_t)0x001f)<<3)

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

int stb_image_draw_get_pixel(stb_image_t *img, int x, int y);
void  stb_image_draw_set_pixel(stb_image_t *img, int x, int y, int c);
void stb_image_draw_point_fill(stb_image_t *img, int cx, int cy, int r0, int r1, int c);
void stb_image_draw_line(stb_image_t *img, int x0, int y0, int x1, int y1, int c, int thickness);
void stb_image_draw_xLine(stb_image_t *img, int x1, int x2, int y, int c);
void stb_image_draw_yLine(stb_image_t *img, int x, int y1, int y2, int c);
void stb_image_draw_rectangle(stb_image_t *img, int rx, int ry, int rw, int rh, int c, int thickness, bool fill);
void stb_image_draw_draw_circle(stb_image_t *img, int cx, int cy, int r, int c, int thickness, bool fill);
void stb_image_draw_cross(stb_image_t *img, int x, int y, int c, int size, int thickness);
void stb_image_scratch_draw_pixel(stb_image_t *img, int x0, int y0, int dx, int dy, float shear_dx, float shear_dy, int r0, int r1, int c);
void stb_image_scratch_draw_line(stb_image_t *img, int x0, int y0, int dx, int dy0, int dy1, float shear_dx, float shear_dy, int c);
void stb_image_scratch_draw_sheared_ellipse(stb_image_t *img, int x0, int y0, int width, int height, bool filled, float shear_dx, float shear_dy, int c, int thickness);
void stb_image_scratch_draw_rotated_ellipse(stb_image_t *img, int x, int y, int x_axis, int y_axis, int rotation, bool filled, int c, int thickness);
void stb_image_draw_ellipse(stb_image_t *img, int cx, int cy, int rx, int ry, int rotation, int c, int thickness, bool fill);
uint32_t stb_image_draw_blend_rgb888(uint32_t fg_color, uint32_t bg_color, uint8_t alpha);
uint16_t stb_image_draw_blend_rgb565(uint16_t fg_color, uint16_t bg_color, uint8_t alpha);


#ifdef STB_IMAGE_DRAW_IMPLEMENTATION
#ifndef STB_IMAGE_DRAW_IMPLEMENTATION_SRC
#define STB_IMAGE_DRAW_IMPLEMENTATION_SRC
uint32_t stb_image_draw_blend_rgb888(uint32_t fg_color, uint32_t bg_color, uint8_t alpha) {

    uint8_t r1 = COLOR_RGB888_TO_R8(fg_color);
    uint8_t g1 = COLOR_RGB888_TO_G8(fg_color);
    uint8_t b1 = COLOR_RGB888_TO_B8(fg_color);

    uint8_t r2 = COLOR_RGB888_TO_R8(bg_color);
    uint8_t g2 = COLOR_RGB888_TO_G8(bg_color);
    uint8_t b2 = COLOR_RGB888_TO_B8(bg_color);

    uint8_t r = (r1 * alpha + r2 * (255 - alpha)) / 255;
    uint8_t g = (g1 * alpha + g2 * (255 - alpha)) / 255;
    uint8_t b = (b1 * alpha + b2 * (255 - alpha)) / 255;

    return COLOR_R8_G8_B8_TO_RGB888(r, g, b);
}

uint16_t stb_image_draw_blend_rgb565(uint16_t fg_color, uint16_t bg_color, uint8_t alpha) {

    uint8_t r1 = COLOR_RGB565_TO_R8(fg_color);
    uint8_t g1 = COLOR_RGB565_TO_G8(fg_color);
    uint8_t b1 = COLOR_RGB565_TO_B8(fg_color);

    uint8_t r2 = COLOR_RGB565_TO_R8(bg_color);
    uint8_t g2 = COLOR_RGB565_TO_G8(bg_color);
    uint8_t b2 = COLOR_RGB565_TO_B8(bg_color);

    uint8_t r = (r1 * alpha + r2 * (255 - alpha)) / 255;
    uint8_t g = (g1 * alpha + g2 * (255 - alpha)) / 255;
    uint8_t b = (b1 * alpha + b2 * (255 - alpha)) / 255;

    return COLOR_R8_G8_B8_TO_RGB565(r, g, b);
}

int stb_image_draw_get_pixel(stb_image_t *img, int x, int y)
{
    if(y >= img->h || x >= img->w) return -1;
    switch (img->format) {
        case PIXFORMAT_RGB565: {
            return (int)((unsigned short *) img->rawdata)[(img->w * y) + x];
        }
        case PIXFORMAT_RGB888: {
            uint8_t *pix = ((uint8_t*)img->rawdata) + (img->w * y + x) * 3;
            return COLOR_R8_G8_B8_TO_RGB888(pix[0], pix[1], pix[2]);
        }
        case PIXFORMAT_BGR888: {
            uint8_t *pix = ((uint8_t*)img->rawdata) + (img->w * y + x) * 3;
            return COLOR_R8_G8_B8_TO_BGR888(pix[2], pix[1], pix[0]);
        }
        default: {
            return -1;
        }
    }
}

void  stb_image_draw_set_pixel(stb_image_t *img, int x, int y, int c)
{
    if(y >= img->h || x >= img->w) return ;
    switch (img->format)
    {
    case PIXFORMAT_RGB565:
        {
            unsigned short *pix = (unsigned short*)img->rawdata;
            pix[img->w * y + x] = (unsigned short)c;
        }
        break; 
    case PIXFORMAT_RGB888:
        {
            uint8_t *pix = ((uint8_t*)img->rawdata) + (img->w * y + x) * 3;
            pix[0] = COLOR_RGB888_TO_R8(c);
            pix[1] = COLOR_RGB888_TO_G8(c);
            pix[2] = COLOR_RGB888_TO_B8(c);
        }
        break;
    case PIXFORMAT_BGR888:
        {
            uint8_t *pix = ((uint8_t*)img->rawdata) + (img->w * y + x) * 3;
            pix[0] = COLOR_BGR888_TO_B8(c);
            pix[1] = COLOR_BGR888_TO_G8(c);
            pix[2] = COLOR_BGR888_TO_R8(c);
        }
        break;
    default:
        return ;
        break;
    }
}

void stb_image_draw_point_fill(stb_image_t *img, int cx, int cy, int r0, int r1, int c)
{
    for (int y = r0; y <= r1; y++) {
        for (int x = r0; x <= r1; x++) {
            if (((x * x) + (y * y)) <= (r0 * r0)) {
                stb_image_draw_set_pixel(img, cx + x, cy + y, c);
            }
        }
    }
}

void stb_image_draw_line(stb_image_t *img, int x0, int y0, int x1, int y1, int c, int thickness)
{
    if (thickness == 1) {
        int dx = abs(x1 - x0), sx = (x0 < x1) ? 1 : -1;
        int dy = abs(y1 - y0), sy = (y0 < y1) ? 1 : -1;
        int err = ((dx > dy) ? dx : -dy) / 2;

        for (;;) {
            stb_image_draw_set_pixel(img, x0, y0, c);
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
            stb_image_draw_point_fill(img, x0, y0, -thickness0, thickness1, c);
            if ((x0 == x1) && (y0 == y1)) break;
            int e2 = err;
            if (e2 > -dx) { err -= dy; x0 += sx; }
            if (e2 <  dy) { err += dx; y0 += sy; }
        }
    }
}

void stb_image_draw_xLine(stb_image_t *img, int x1, int x2, int y, int c)
{
    while (x1 <= x2) stb_image_draw_set_pixel(img, x1++, y, c);
}

void stb_image_draw_yLine(stb_image_t *img, int x, int y1, int y2, int c)
{
    while (y1 <= y2) stb_image_draw_set_pixel(img, x, y1++, c);
}

void stb_image_draw_rectangle(stb_image_t *img, int rx, int ry, int rw, int rh, int c, int thickness, bool fill)
{
    if (fill) {
        for (int y = ry, yy = ry + rh; y < yy; y++) {
            for (int x = rx, xx = rx + rw; x < xx; x++) {
                stb_image_draw_set_pixel(img, x, y, c);
            }
        }
    } else if (thickness > 0) {
        int thickness0 = (thickness - 0) / 2;
        int thickness1 = (thickness - 1) / 2;

        for (int i = rx - thickness0, j = rx + rw + thickness1, k = ry + rh - 1; i < j; i++) {
            stb_image_draw_yLine(img, i, ry - thickness0, ry + thickness1, c);
            stb_image_draw_yLine(img, i, k - thickness0, k + thickness1, c);
        }

        for (int i = ry - thickness0, j = ry + rh + thickness1, k = rx + rw - 1; i < j; i++) {
            stb_image_draw_xLine(img, rx - thickness0, rx + thickness1, i, c);
            stb_image_draw_xLine(img, k - thickness0, k + thickness1, i, c);
        }
    }
}
void stb_image_draw_draw_circle(stb_image_t *img, int cx, int cy, int r, int c, int thickness, bool fill)
{
    if (fill) {
        stb_image_draw_point_fill(img, cx, cy, -r, r, c);
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
            stb_image_draw_xLine(img, cx + xi, cx + xo, cy + y,  c);
            stb_image_draw_yLine(img, cx + y,  cy + xi, cy + xo, c);
            stb_image_draw_xLine(img, cx - xo, cx - xi, cy + y,  c);
            stb_image_draw_yLine(img, cx - y,  cy + xi, cy + xo, c);
            stb_image_draw_xLine(img, cx - xo, cx - xi, cy - y,  c);
            stb_image_draw_yLine(img, cx - y,  cy - xo, cy - xi, c);
            stb_image_draw_xLine(img, cx + xi, cx + xo, cy - y,  c);
            stb_image_draw_yLine(img, cx + y,  cy - xo, cy - xi, c);

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

void stb_image_draw_cross(stb_image_t *img, int x, int y, int c, int size, int thickness)
{
    stb_image_draw_line(img, x - size, y        , x + size, y        , c, thickness);
    stb_image_draw_line(img, x        , y - size, x        , y + size, c, thickness);
}

void stb_image_scratch_draw_pixel(stb_image_t *img, int x0, int y0, int dx, int dy, float shear_dx, float shear_dy, int r0, int r1, int c)
{
    int _roundf = (int)((float)(((float)dx * (float)shear_dy) / (float)shear_dx) + 0.5f);
    stb_image_draw_point_fill(img, x0 + dx, y0 + dy + _roundf, r0, r1, c);
}

void stb_image_scratch_draw_line(stb_image_t *img, int x0, int y0, int dx, int dy0, int dy1, float shear_dx, float shear_dy, int c)
{
    int y = y0 +  (int)((float)(((float)dx * (float)shear_dy) / (float)shear_dx) + 0.5f);
    stb_image_draw_yLine(img, x0 + dx, y + dy0, y + dy1, c);
}

void stb_image_scratch_draw_sheared_ellipse(stb_image_t *img, int x0, int y0, int width, int height, bool filled, float shear_dx, float shear_dy, int c, int thickness)
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
                stb_image_scratch_draw_line(img, x0, y0, x, -y, y, shear_dx, shear_dy, c);
                stb_image_scratch_draw_line(img, x0, y0, -x, -y, y, shear_dx, shear_dy, c);
            } else {
                stb_image_scratch_draw_pixel(img, x0, y0, x, y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, -x, y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, x, -y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, -x, -y, shear_dx, shear_dy, -thickness0, thickness1, c);
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
                stb_image_scratch_draw_line(img, x0, y0, x, -y, y, shear_dx, shear_dy, c);
                stb_image_scratch_draw_line(img, x0, y0, -x, -y, y, shear_dx, shear_dy, c);
            } else {
                stb_image_scratch_draw_pixel(img, x0, y0, x, y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, -x, y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, x, -y, shear_dx, shear_dy, -thickness0, thickness1, c);
                stb_image_scratch_draw_pixel(img, x0, y0, -x, -y, shear_dx, shear_dy, -thickness0, thickness1, c);
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

void stb_image_scratch_draw_rotated_ellipse(stb_image_t *img, int x, int y, int x_axis, int y_axis, int rotation, bool filled, int c, int thickness)
{
    if ((x_axis > 0) && (y_axis > 0)) {
        if ((x_axis == y_axis) || (rotation == 0)) {
            stb_image_scratch_draw_sheared_ellipse(img, x, y, x_axis / 2, y_axis / 2, filled, 1, 0, c, thickness);
        } else if (rotation == 90) {
            stb_image_scratch_draw_sheared_ellipse(img, x, y, y_axis / 2, x_axis / 2, filled, 1, 0, c, thickness);
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
            stb_image_scratch_draw_sheared_ellipse(img, x, y, (int)floorf(shear_x_axis / 2), (int)floorf(shear_y_axis / 2), filled, shear_dx, shear_dy, c, thickness);
        }
    }
}

void stb_image_draw_ellipse(stb_image_t *img, int cx, int cy, int rx, int ry, int rotation, int c, int thickness, bool fill)
{
    int r = rotation % 180;
    if (r < 0) r += 180;

    stb_image_scratch_draw_rotated_ellipse(img, cx, cy, rx * 2, ry * 2, r, fill, c, thickness);
}
// static void stb_image_draw_flood_fill(stb_image_t *img, int x, int y,
//                       float seed_threshold, float floating_threshold,
//                       int c, bool invert, bool clear_background, void *mask);

// void imlib_draw_string(void *img, int x_off, int y_off, const char *str, int c, float scale, int x_spacing, int y_spacing, bool mono_space,
//                        int char_rotation, bool char_hmirror, bool char_vflip, int string_rotation, bool string_hmirror, bool string_hflip);
// void imlib_draw_image_fast(void *img, void *other, int x_off, int y_off, float x_scale, float y_scale, float alpha, void *mask);
#endif
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
