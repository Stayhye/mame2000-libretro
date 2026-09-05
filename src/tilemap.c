#include <string.h>
#include <stdint.h>

// Optimized MIPS R5900 (PlayStation 2) implementation for tilemap mask generation and drawing routines.
// This version minimizes redundant pointer dereferences, optimizes loop bounds, and utilizes 
// direct register-friendly types to accelerate software tilemap decoding and update passes.

static int draw_color_mask(
    struct osd_bitmap *mask,
    uint32_t col, uint32_t row,
    uint32_t tile_width, uint32_t tile_height,
    const uint8_t *pendata,
    const uint16_t *clut,
    int transparent_color,
    uint32_t flags )
{
    int is_opaque = 1, is_transparent = 1;
    int x, bit, sx = tile_width * col;
    int sy, y1, y2, dy;

    if ( flags & TILE_FLIPY ){
        y1 = tile_height * row + tile_height - 1;
        y2 = y1 - tile_height;
        dy = -1;
    }
    else {
        y1 = tile_height * row;
        y2 = y1 + tile_height;
        dy = 1;
    }

    if ( flags & TILE_FLIPX ){
        tile_width--;
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = tw_bytes; x >= 0; x-- ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data >>= 1;
                    if ( clut[pen] != transparent_color ) data |= 0x80;
                }
                if ( data != 0x00 ) is_transparent = 0;
                if ( data != 0xff ) is_opaque = 0;
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
    else {
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = 0; x < tw_bytes; x++ ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data <<= 1;
                    if ( clut[pen] != transparent_color ) data |= 0x01;
                }
                if ( data != 0x00 ) is_transparent = 0;
                if ( data != 0xff ) is_opaque = 0;
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
    if ( is_transparent ) return TILE_TRANSPARENT;
    if ( is_opaque ) return TILE_OPAQUE;
    return TILE_MASKED;
}

static int draw_pen_mask(
    struct osd_bitmap *mask,
    uint32_t col, uint32_t row,
    uint32_t tile_width, uint32_t tile_height,
    const uint8_t *pendata,
    int transparent_pen,
    uint32_t flags )
{
    int is_opaque = 1, is_transparent = 1;
    int x, bit, sx = tile_width * col;
    int sy, y1, y2, dy;

    if ( flags & TILE_FLIPY ){
        y1 = tile_height * row + tile_height - 1;
        y2 = y1 - tile_height;
        dy = -1;
    }
    else {
        y1 = tile_height * row;
        y2 = y1 + tile_height;
        dy = 1;
    }

    if ( flags & TILE_FLIPX ){
        tile_width--;
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = tw_bytes; x >= 0; x-- ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data >>= 1;
                    if ( (int)pen != transparent_pen ) data |= 0x80;
                }
                if ( data != 0x00 ) is_transparent = 0;
                if ( data != 0xff ) is_opaque = 0;
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
    else {
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = 0; x < tw_bytes; x++ ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data <<= 1;
                    if ( (int)pen != transparent_pen ) data |= 0x01;
                }
                if ( data != 0x00 ) is_transparent = 0;
                if ( data != 0xff ) is_opaque = 0;
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
    if ( is_transparent ) return TILE_TRANSPARENT;
    if ( is_opaque ) return TILE_OPAQUE;
    return TILE_MASKED;
}

static void draw_mask(
    struct osd_bitmap *mask,
    uint32_t col, uint32_t row,
    uint32_t tile_width, uint32_t tile_height,
    const uint8_t *pendata,
    uint32_t transmask,
    uint32_t flags )
{
    int x, bit, sx = tile_width * col;
    int sy, y1, y2, dy;

    if ( flags & TILE_FLIPY ){
        y1 = tile_height * row + tile_height - 1;
        y2 = y1 - tile_height;
        dy = -1;
    }
    else {
        y1 = tile_height * row;
        y2 = y1 + tile_height;
        dy = 1;
    }

    if ( flags & TILE_FLIPX ){
        tile_width--;
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = tw_bytes; x >= 0; x-- ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data >>= 1;
                    if ( !((1U << pen) & transmask) ) data |= 0x80;
                }
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
    else {
        int tw_bytes = tile_width >> 3;
        for ( sy = y1; sy != y2; sy += dy ){
            uint8_t *mask_dest = mask->line[sy] + (sx >> 3);
            for ( x = 0; x < tw_bytes; x++ ){
                uint32_t data = 0;
                for ( bit = 0; bit < 8; bit++ ){
                    uint32_t pen = *pendata++;
                    data <<= 1;
                    if ( !((1U << pen) & transmask) ) data |= 0x01;
                }
                mask_dest[x] = (uint8_t)data;
            }
        }
    }
}

static void render_mask( struct tilemap *tilemap, uint32_t cached_index ){
    const struct cached_tile_info *cached_tile_info = &tilemap->cached_tile_info[cached_index];
    uint32_t col = cached_index % tilemap->num_cached_cols;
    uint32_t row = cached_index / tilemap->num_cached_cols;
    uint32_t type = tilemap->type;

    uint32_t transparent_pen = tilemap->transparent_pen;
    unsigned int *transmask = tilemap->transmask;
    uint32_t tile_width = tilemap->cached_tile_width;
    uint32_t tile_height = tilemap->cached_tile_height;

    uint32_t pen_usage = cached_tile_info->pen_usage;
    const uint8_t *pen_data = cached_tile_info->pen_data;
    uint32_t flags = cached_tile_info->flags;

    if ( type & TILEMAP_BITMASK ){
        tilemap->foreground->data_row[row][col] =
            draw_bitmask( tilemap->foreground->bitmask, col, row,
                tile_width, tile_height, tile_info.mask_data, flags );
    }
    else if ( type & TILEMAP_SPLIT ){
        uint32_t pen_mask = ((int32_t)transparent_pen < 0) ? 0 : (1U << transparent_pen);
        if ( flags & TILE_IGNORE_TRANSPARENCY ){
            tilemap->foreground->data_row[row][col] = TILE_OPAQUE;
            tilemap->background->data_row[row][col] = TILE_OPAQUE;
        }
        else if ( pen_mask == pen_usage ){ /* totally transparent */
            tilemap->foreground->data_row[row][col] = TILE_TRANSPARENT;
            tilemap->background->data_row[row][col] = TILE_TRANSPARENT;
        }
        else {
            uint32_t fg_transmask = transmask[(flags >> 2) & 3];
            uint32_t bg_transmask = (~fg_transmask) | pen_mask;
            if ( (pen_usage & fg_transmask) == 0 ){ /* foreground totally opaque */
                tilemap->foreground->data_row[row][col] = TILE_OPAQUE;
                tilemap->background->data_row[row][col] = TILE_TRANSPARENT;
            }
            else if ( (pen_usage & bg_transmask) == 0 ){ /* background totally opaque */
                tilemap->foreground->data_row[row][col] = TILE_TRANSPARENT;
                tilemap->background->data_row[row][col] = TILE_OPAQUE;
            }
            else if ( (pen_usage & ~bg_transmask) == 0 ){ /* background transparent */
                draw_mask( tilemap->foreground->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data, fg_transmask, flags );
                tilemap->foreground->data_row[row][col] = TILE_MASKED;
                tilemap->background->data_row[row][col] = TILE_TRANSPARENT;
            }
            else if ( (pen_usage & ~fg_transmask) == 0 ){ /* foreground transparent */
                draw_mask( tilemap->background->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data, bg_transmask, flags );
                tilemap->foreground->data_row[row][col] = TILE_TRANSPARENT;
                tilemap->background->data_row[row][col] = TILE_MASKED;
            }
            else { /* split tile - opacity in both foreground and background */
                draw_mask( tilemap->foreground->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data, fg_transmask, flags );
                draw_mask( tilemap->background->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data, bg_transmask, flags );
                tilemap->foreground->data_row[row][col] = TILE_MASKED;
                tilemap->background->data_row[row][col] = TILE_MASKED;
            }
        }
    }
    else if ( type == TILEMAP_TRANSPARENT ){
        if ( pen_usage ){
            uint32_t fg_transmask = 1U << transparent_pen;
            if ( flags & TILE_IGNORE_TRANSPARENCY ) fg_transmask = 0;
            if ( pen_usage == fg_transmask ){
                tilemap->foreground->data_row[row][col] = TILE_TRANSPARENT;
            }
            else if ( pen_usage & fg_transmask ){
                draw_mask( tilemap->foreground->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data, fg_transmask, flags );
                tilemap->foreground->data_row[row][col] = TILE_MASKED;
            }
            else {
                tilemap->foreground->data_row[row][col] = TILE_OPAQUE;
            }
        }
        else {
            tilemap->foreground->data_row[row][col] =
                draw_pen_mask(
                    tilemap->foreground->bitmask,
                    col, row, tile_width, tile_height,
                    pen_data,
                    (int)transparent_pen,
                    flags
                );
        }
    }
    else if ( type == TILEMAP_TRANSPARENT_COLOR ){
        tilemap->foreground->data_row[row][col] =
            draw_color_mask(
                tilemap->foreground->bitmask,
                col, row, tile_width, tile_height,
                pen_data,
                Machine->game_colortable +
                    (cached_tile_info->pal_data - Machine->remapped_colortable),
                (int)transparent_pen,
                flags
            );
    }
    else {
        tilemap->foreground->data_row[row][col] = TILE_OPAQUE;
    }
}

static void update_tile_info( struct tilemap *tilemap ){
    int *logical_flip_to_cached_flip = tilemap->logical_flip_to_cached_flip;
    uint32_t num_pens = tilemap->cached_tile_width * tilemap->cached_tile_height;
    uint32_t num_tiles = tilemap->num_tiles;
    uint32_t cached_index;
    uint8_t *visible = tilemap->visible;
    uint8_t *dirty_vram = tilemap->dirty_vram;
    uint8_t *dirty_pixels = tilemap->dirty_pixels;
    
    tile_info.flags = 0;
    tile_info.priority = 0;
    
    for ( cached_index = 0; cached_index < num_tiles; cached_index++ ){
        if ( visible[cached_index] && dirty_vram[cached_index] ){
            struct cached_tile_info *cached_tile_info = &tilemap->cached_tile_info[cached_index];
            uint32_t memory_offset = tilemap->cached_index_to_memory_offset[cached_index];
            unregister_pens( cached_tile_info, num_pens );
            tilemap->tile_get_info( memory_offset );
            {
                uint32_t flags = tile_info.flags;
                cached_tile_info->flags = (flags & 0xfc) | logical_flip_to_cached_flip[flags & 0x3];
            }
            cached_tile_info->pen_usage = tile_info.pen_usage;
            cached_tile_info->pen_data = tile_info.pen_data;
            cached_tile_info->pal_data = tile_info.pal_data;
            tilemap->priority[cached_index] = tile_info.priority;
            register_pens( cached_tile_info, num_pens );
            dirty_pixels[cached_index] = 1;
            dirty_vram[cached_index] = 0;
            render_mask( tilemap, cached_index );
        }
    }
}

static void update_visible( struct tilemap *tilemap ){
    memset( tilemap->visible, 1, tilemap->num_tiles );
}

void tilemap_update( struct tilemap *tilemap ){
    profiler_mark( PROFILER_TILEMAP_UPDATE );
    if ( tilemap == ALL_TILEMAPS ){
        tilemap = first_tilemap;
        while ( tilemap ){
            tilemap_update( tilemap );
            tilemap = tilemap->next;
        }
    }
    else if ( tilemap->enable ){
        update_visible( tilemap );
        update_tile_info( tilemap );
    }
    profiler_mark( PROFILER_END );
}

/***********************************************************************************/

void tilemap_set_scrolldx( struct tilemap *tilemap, int dx, int dx_if_flipped ){
    tilemap->dx = dx;
    tilemap->dx_if_flipped = dx_if_flipped;
    tilemap->scrollx_delta = ( tilemap->attributes & TILEMAP_FLIPX ) ? dx_if_flipped : dx;
}

void tilemap_set_scrolldy( struct tilemap *tilemap, int dy, int dy_if_flipped ){
    tilemap->dy = dy;
    tilemap->dy_if_flipped = dy_if_flipped;
    tilemap->scrolly_delta = ( tilemap->attributes & TILEMAP_FLIPY ) ? dy_if_flipped : dy;
}

void tilemap_set_scrollx( struct tilemap *tilemap, int which, int value ){
    value = tilemap->scrollx_delta - value;

    if ( tilemap->orientation & ORIENTATION_SWAP_XY ){
        if ( tilemap->orientation & ORIENTATION_FLIP_X ) which = tilemap->scroll_cols - 1 - which;
        if ( tilemap->orientation & ORIENTATION_FLIP_Y ) value = screen_height - tilemap->cached_height - value;
        if ( tilemap->colscroll[which] != value ){
            tilemap->colscroll[which] = value;
        }
    }
    else {
        if ( tilemap->orientation & ORIENTATION_FLIP_Y ) which = tilemap->scroll_rows - 1 - which;
        if ( tilemap->orientation & ORIENTATION_FLIP_X ) value = screen_width - tilemap->cached_width - value;
        if ( tilemap->rowscroll[which] != value ){
            tilemap->rowscroll[which] = value;
        }
    }
}

void tilemap_set_scrolly( struct tilemap *tilemap, int which, int value ){
    value = tilemap->scrolly_delta - value;

    if ( tilemap->orientation & ORIENTATION_SWAP_XY ){
        if ( tilemap->orientation & ORIENTATION_FLIP_Y ) which = tilemap->scroll_rows - 1 - which;
        if ( tilemap->orientation & ORIENTATION_FLIP_X ) value = screen_width - tilemap->cached_width - value;
        if ( tilemap->rowscroll[which] != value ){
            tilemap->rowscroll[which] = value;
        }
    }
    else {
        if ( tilemap->orientation & ORIENTATION_FLIP_X ) which = tilemap->scroll_cols - 1 - which;
        if ( tilemap->orientation & ORIENTATION_FLIP_Y ) value = screen_height - tilemap->cached_height - value;
        if ( tilemap->colscroll[which] != value ){
            tilemap->colscroll[which] = value;
        }
    }
}

/***********************************************************************************/

void tilemap_draw( struct osd_bitmap *dest, struct tilemap *tilemap, uint32_t priority ){
    int xpos, ypos;

    profiler_mark( PROFILER_TILEMAP_DRAW );
    if ( tilemap->enable ){
        void (*draw)( int, int );

        int rows = tilemap->scroll_rows;
        const int *rowscroll = tilemap->rowscroll;
        int cols = tilemap->scroll_cols;
        const int *colscroll = tilemap->colscroll;

        int left = tilemap->clip_left;
        int right = tilemap->clip_right;
        int top = tilemap->clip_top;
        int bottom = tilemap->clip_bottom;

        int tile_height = tilemap->cached_tile_height;

        blit.screen = dest;
        blit.dest_line_offset = dest->line[1] - dest->line[0];

        blit.pixmap = tilemap->pixmap;
        blit.source_line_offset = tilemap->pixmap_line_offset;

        if ( tilemap->type == TILEMAP_OPAQUE || (priority & TILEMAP_IGNORE_TRANSPARENCY) ){
            draw = tilemap->draw_opaque;
        }
        else {
            draw = tilemap->draw;
            if ( priority & TILEMAP_BACK ){
                blit.bitmask = tilemap->background->bitmask;
                blit.mask_line_offset = tilemap->background->line_offset;
                blit.mask_data_row = tilemap->background->data_row;
            }
            else {
                blit.bitmask = tilemap->foreground->bitmask;
                blit.mask_line_offset = tilemap->foreground->line_offset;
                blit.mask_data_row = tilemap->foreground->data_row;
            }

            blit.mask_row_offset = tile_height * blit.mask_line_offset;
        }

        if ( dest->depth == 16 ){
            blit.dest_line_offset >>= 1;
            blit.source_line_offset >>= 1;
        }

        blit.source_row_offset = tile_height * blit.source_line_offset;
        blit.dest_row_offset = tile_height * blit.dest_line_offset;

        blit.priority_data_row = tilemap->priority_row;
        blit.source_width = tilemap->cached_width;
        blit.source_height = tilemap->cached_height;
        blit.tile_priority = priority & 0xf;
        blit.tilemap_priority_code = priority >> 16;

        if ( rows == 1 && cols == 1 ){ /* XY scrolling playfield */
            int scrollx = rowscroll[0];
            int scrolly = colscroll[0];

            if ( scrollx < 0 ){
                scrollx = blit.source_width - (-scrollx) % blit.source_width;
            }
            else {
                scrollx = scrollx % blit.source_width;
            }

            if ( scrolly < 0 ){
                scrolly = blit.source_height - (-scrolly) % blit.source_height;
            }
            else {
                scrolly = scrolly % blit.source_height;
            }

            blit.clip_left = left;
            blit.clip_top = top;
            blit.clip_right = right;
            blit.clip_bottom = bottom;

            for (
                ypos = scrolly - blit.source_height;
                ypos < blit.clip_bottom;
                ypos += blit.source_height
            ){
                for (
                    xpos = scrollx - blit.source_width;
                    xpos < blit.clip_right;
                    xpos += blit.source_width
                ){
                    draw( xpos, ypos );
                }
            }
        }
        else if ( rows == 1 ){ /* scrolling columns + horizontal scroll */
            int col = 0;
            int colwidth = blit.source_width / cols;
            int scrollx = rowscroll[0];

            if ( scrollx < 0 ){
                scrollx = blit.source_width - (-scrollx) % blit.source_width;
            }
            else {
                scrollx = scrollx % blit.source_width;
            }

            blit.clip_top = top;
            blit.clip_bottom = bottom;

            while ( col < cols ){
                int cons = 1;
                int scrolly = colscroll[col];

                if ( scrolly != TILE_LINE_DISABLED ){
                    while ( col + cons < cols && colscroll[col + cons] == scrolly ) cons++;

                    if ( scrolly < 0 ){
                        scrolly = blit.source_height - (-scrolly) % blit.source_height;
                    }
                    else {
                        scrolly %= blit.source_height;
                    }

                    blit.clip_left = col * colwidth + scrollx;
                    if ( blit.clip_left < left ) blit.clip_left = left;
                    blit.clip_right = (col + cons) * colwidth + scrollx;
                    if ( blit.clip_right > right ) blit.clip_right = right;

                    for (
                        ypos = scrolly - blit.source_height;
                        ypos < blit.clip_bottom;
                        ypos += blit.source_height
                    ){
                        draw( scrollx, ypos );
                    }

                    blit.clip_left = col * colwidth + scrollx - blit.source_width;
                    if ( blit.clip_left < left ) blit.clip_left = left;
                    blit.clip_right = (col + cons) * colwidth + scrollx - blit.source_width;
                    if ( blit.clip_right > right ) blit.clip_right = right;

                    for (
                        ypos = scrolly - blit.source_height;
                        ypos < blit.clip_bottom;
                        ypos += blit.source_height
                    ){
                        draw( scrollx - blit.source_width, ypos );
                    }
                }
                col += cons;
            }
        }
        else if ( cols == 1 ){ /* scrolling rows + vertical scroll */
            int row = 0;
            int rowheight = blit.source_height / rows;
            int scrolly = colscroll[0];
            if ( scrolly < 0 ){
                scrolly = blit.source_height - (-scrolly) % blit.source_height;
            }
            else {
                scrolly = scrolly % blit.source_height;
            }
            blit.clip_left = left;
            blit.clip_right = right;
            while ( row < rows ){
                int cons = 1;
                int scrollx = rowscroll[row];
                if ( scrollx != TILE_LINE_DISABLED ){
                    while ( row + cons < rows && rowscroll[row + cons] == scrollx ) cons++;
                    if ( scrollx < 0 ){
                        scrollx = blit.source_width - (-scrollx) % blit.source_width;
                    }
                    else {
                        scrollx %= blit.source_width;
                    }
                    blit.clip_top = row * rowheight + scrolly;
                    if ( blit.clip_top < top ) blit.clip_top = top;
                    blit.clip_bottom = (row + cons) * rowheight + scrolly;
                    if ( blit.clip_bottom > bottom ) blit.clip_bottom = bottom;
                    for (
                        xpos = scrollx - blit.source_width;
                        xpos < blit.clip_right;
                        xpos += blit.source_width
                    ){
                        draw( xpos, scrolly );
                    }
                    blit.clip_top = row * rowheight + scrolly - blit.source_height;
                    if ( blit.clip_top < top ) blit.clip_top = top;
                    blit.clip_bottom = (row + cons) * rowheight + scrolly - blit.source_height;
                    if ( blit.clip_bottom > bottom ) blit.clip_bottom = bottom;
                    for (
                        xpos = scrollx - blit.source_width;
                        xpos < blit.clip_right;
                        xpos += blit.source_width
                    ){
                        draw( xpos, scrolly - blit.source_height );
                    }
                }
                row += cons;
            }
        }
    }
    profiler_mark( PROFILER_END );
}