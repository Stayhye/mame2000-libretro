#ifndef ARTWORK_H
#define ARTWORK_H

/* Global flag variables referenced in mame.c and vector.o */
extern int artwork_overlay;
extern int artwork_backdrop;

/* Real screen bitmap pointer used for artwork overlays */
extern struct osd_bitmap *overlay_real_scrbitmap;

/* Inline stubs for artwork functions */
#define artwork_init()              (0)
#define artwork_exit()              ((void)0)
#define overlay_free()              ((void)0)
#define backdrop_free()             ((void)0)
#define overlay_draw(...)           ((void)0)
#define overlay_remap()             ((void)0)

#endif