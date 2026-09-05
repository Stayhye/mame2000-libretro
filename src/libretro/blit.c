#include "libretro.h"
#include "driver.h"
#include "dirty.h"
#include <stdint.h>

/* from video.c */
extern char *dirty_old;
extern char *dirty_new;
extern int gfx_xoffset;
extern int gfx_yoffset;
extern int gfx_display_lines;
extern int gfx_display_columns;
extern int gfx_width;
extern int gfx_height;
extern int skiplines;
extern int skipcolumns;

#define SCREEN16 gp2x_screen15

#include "minimal.h"

uint32_t *palette_16bit_lookup;

/* Unroll factor optimized to exploit R5900 pipeline concurrency and 
 * keep memory accesses aligned within cache lines. */
#define BLIT_UNROLL 4

void blitscreen_dirty1_color8(struct osd_bitmap *bitmap)
{
    int x, y;
    int width=(bitmap->line[1] - bitmap->line[0]);
    unsigned char *lb=bitmap->line[skiplines] + skipcolumns;
    unsigned short *address=SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);
    const unsigned short * __restrict__ pal = gp2x_palette;

    for (y = 0; y < gfx_display_lines; y += 16)
    {
        for (x = 0; x < gfx_display_columns; )
        {
            int w = 16;
            if (ISDIRTY(x,y))
            {
                int h;
                unsigned char  * __restrict__ lb0      = lb + x;
                unsigned short * __restrict__ address0 = address + x;
                while (x + w < gfx_display_columns && ISDIRTY(x+w,y))
                            w += 16;
                if (x + w > gfx_display_columns)
                            w = gfx_display_columns - x;
                for (h = 0; ((h < 16) && ((y + h) < gfx_display_lines)); h++)
                {
                    int wx;
                    for (wx = 0; wx + BLIT_UNROLL <= w; wx += BLIT_UNROLL)
                    {
                        unsigned p0 = lb0[wx + 0];
                        unsigned p1 = lb0[wx + 1];
                        unsigned p2 = lb0[wx + 2];
                        unsigned p3 = lb0[wx + 3];
                        address0[wx + 0] = pal[p0];
                        address0[wx + 1] = pal[p1];
                        address0[wx + 2] = pal[p2];
                        address0[wx + 3] = pal[p3];
                    }
                    for (; wx < w; wx++)
                        address0[wx] = pal[lb0[wx]];
                    lb0 += width;
                    address0 += gfx_width;
                }
            }
            x += w;
            }
        lb += 16 * width;
        address += 16 * gfx_width;
    }
}

void blitscreen_dirty0_color8(struct osd_bitmap *bitmap)
{
    int x,y;
    int width=(bitmap->line[1] - bitmap->line[0]);
    int columns=gfx_display_columns;
    unsigned char  * __restrict__ lb      = bitmap->line[skiplines] + skipcolumns;
    unsigned short * __restrict__ address = SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);
    const unsigned short * __restrict__ pal = gp2x_palette;

    for (y = 0; y < gfx_display_lines; y++)
    {
        for (x = 0; x + BLIT_UNROLL <= columns; x += BLIT_UNROLL)
        {
            unsigned p0 = lb[x + 0];
            unsigned p1 = lb[x + 1];
            unsigned p2 = lb[x + 2];
            unsigned p3 = lb[x + 3];
            address[x + 0] = pal[p0];
            address[x + 1] = pal[p1];
            address[x + 2] = pal[p2];
            address[x + 3] = pal[p3];
        }
        for (; x < columns; x++)
            address[x] = pal[lb[x]];
        lb+=width;
        address+=gfx_width;
    }
}

void blitscreen_dirty1_palettized16(struct osd_bitmap *bitmap)
{
    int x, y;
    int width=(bitmap->line[1] - bitmap->line[0])>>1;
    unsigned short *lb=((unsigned short*)(bitmap->line[skiplines])) + skipcolumns;
    unsigned short *address=SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);
    const uint32_t * __restrict__ pal = palette_16bit_lookup;

    for (y = 0; y < gfx_display_lines; y += 16)
    {
        for (x = 0; x < gfx_display_columns; )
        {
            int w = 16;
            if (ISDIRTY(x,y))
            {
                int h;
                unsigned short * __restrict__ lb0      = lb + x;
                unsigned short * __restrict__ address0 = address + x;
                while (x + w < gfx_display_columns && ISDIRTY(x+w,y))
                            w += 16;
                if (x + w > gfx_display_columns)
                            w = gfx_display_columns - x;
                for (h = 0; ((h < 16) && ((y + h) < gfx_display_lines)); h++)
                {
                    int wx;
                    for (wx = 0; wx + BLIT_UNROLL <= w; wx += BLIT_UNROLL)
                    {
                        unsigned p0 = lb0[wx + 0];
                        unsigned p1 = lb0[wx + 1];
                        unsigned p2 = lb0[wx + 2];
                        unsigned p3 = lb0[wx + 3];
                        address0[wx + 0] = (unsigned short) pal[p0];
                        address0[wx + 1] = (unsigned short) pal[p1];
                        address0[wx + 2] = (unsigned short) pal[p2];
                        address0[wx + 3] = (unsigned short) pal[p3];
                    }
                    for (; wx < w; wx++)
                        address0[wx] = (unsigned short) pal[lb0[wx]];
                    lb0 += width;
                    address0 += gfx_width;
                }
            }
            x += w;
            }
        lb += 16 * width;
        address += 16 * gfx_width;
    }
}

void blitscreen_dirty0_palettized16(struct osd_bitmap *bitmap)
{
    int x,y;
    int width=(bitmap->line[1] - bitmap->line[0])>>1;
    int columns=gfx_display_columns;
    unsigned short * __restrict__ lb      = ((unsigned short*)(bitmap->line[skiplines])) + skipcolumns;
    unsigned short * __restrict__ address = SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);
    const uint32_t * __restrict__ pal = palette_16bit_lookup;

    for (y = 0; y < gfx_display_lines; y++)
    {
        for (x = 0; x + BLIT_UNROLL <= columns; x += BLIT_UNROLL)
        {
            unsigned p0 = lb[x + 0];
            unsigned p1 = lb[x + 1];
            unsigned p2 = lb[x + 2];
            unsigned p3 = lb[x + 3];
            address[x + 0] = (unsigned short) pal[p0];
            address[x + 1] = (unsigned short) pal[p1];
            address[x + 2] = (unsigned short) pal[p2];
            address[x + 3] = (unsigned short) pal[p3];
        }
        for (; x < columns; x++)
            address[x] = (unsigned short) pal[lb[x]];
        lb+=width;
        address+=gfx_width;
    }
}

void blitscreen_dirty1_color16(struct osd_bitmap *bitmap)
{
    int x, y;
    int width=(bitmap->line[1] - bitmap->line[0])>>1;
    unsigned short *lb=((unsigned short*)(bitmap->line[skiplines])) + skipcolumns;
    unsigned short *address=SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);

    for (y = 0; y < gfx_display_lines; y += 16)
    {
        for (x = 0; x < gfx_display_columns; )
        {
            int w = 16;
            if (ISDIRTY(x,y))
            {
                int h;
                unsigned short * __restrict__ lb0 = lb + x;
                unsigned short * __restrict__ address0 = address + x;
                while (x + w < gfx_display_columns && ISDIRTY(x+w,y))
                            w += 16;
                if (x + w > gfx_display_columns)
                            w = gfx_display_columns - x;
                
                for (h = 0; ((h < 16) && ((y + h) < gfx_display_lines)); h++)
                {
                    int total_bytes = w * sizeof(unsigned short);
                    int chunks = total_bytes >> 3; // 8 bytes per 64-bit block (4 pixels)
                    int rem = total_bytes & 7;

                    const uint64_t *s64 = (const uint64_t *)lb0;
                    uint64_t *d64       = (uint64_t *)address0;

                    int i = 0;
                    for (; i <= chunks - 4; i += 4)
                    {
                        d64[i]     = s64[i];
                        d64[i + 1] = s64[i + 1];
                        d64[i + 2] = s64[i + 2];
                        d64[i + 3] = s64[i + 3];
                    }
                    for (; i < chunks; i++)
                    {
                        d64[i] = s64[i];
                    }

                    if (rem)
                    {
                        const uint8_t *s8 = (const uint8_t *)lb0;
                        uint8_t *d8       = (uint8_t *)address0;
                        int offset        = chunks << 3;
                        for (int b = 0; b < rem; b++)
                        {
                            d8[offset + b] = s8[offset + b];
                        }
                    }

                    lb0 += width;
                    address0 += gfx_width;
                }
            }
            x += w;
            }
        lb += 16 * width;
        address += 16 * gfx_width;
    }
}

void blitscreen_dirty0_color16(struct osd_bitmap *bitmap)
{
    int y;
    int width=(bitmap->line[1] - bitmap->line[0])>>1;
    int columns=gfx_display_columns;
    unsigned short * __restrict__ lb = ((unsigned short*)(bitmap->line[skiplines])) + skipcolumns;
    unsigned short * __restrict__ address = SCREEN16 + gfx_xoffset + (gfx_yoffset * gfx_width);

    int total_bytes = columns * sizeof(unsigned short);
    int chunks = total_bytes >> 3;
    int rem = total_bytes & 7;

    for (y = 0; y < gfx_display_lines; y++)
    {
        const uint64_t *s64 = (const uint64_t *)lb;
        uint64_t *d64       = (uint64_t *)address;

        int i = 0;
        for (; i <= chunks - 4; i += 4)
        {
            d64[i]     = s64[i];
            d64[i + 1] = s64[i + 1];
            d64[i + 2] = s64[i + 2];
            d64[i + 3] = s64[i + 3];
        }
        for (; i < chunks; i++)
        {
            d64[i] = s64[i];
        }

        if (rem)
        {
            const uint8_t *s8 = (const uint8_t *)lb;
            uint8_t *d8       = (uint8_t *)address;
            int offset        = chunks << 3;
            for (int b = 0; b < rem; b++)
            {
                d8[offset + b] = s8[offset + b];
            }
        }

        lb += width;
        address += gfx_width;
    }
}