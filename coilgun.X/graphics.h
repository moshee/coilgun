#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include <stdint.h>
#include <stdbool.h>

#define DISP_W 128
#define DISP_H 8

/**
 * stores font serially where a glyph is a run of bytes representing consecutive
 * columns of the glyph.
 *
 * caveats:
 * - assumes 8-bit ascii character map with no gaps (empty glyphs ok)
*/
struct font {
	int width; // width of the character in pixels = bits
	int height; // height of the character in pixels = bits (should be multiple of 8)
	const uint8_t *mask; // bit mask that applies to each column of the char
	const uint8_t *buf;
};

static inline const uint8_t *font_lookup(const struct font *f, char ch) {
	int off = (int)ch * f->width;
	return f->buf + off;
}

/**
 * Holds 1bpp data for one frame. Data is arranged as sequential segments of 1
 * bit wide by 8 bits tall, laid out in a row (DISP_W), and then stacked
 * vertically (DISP_H).
 */
struct framebuffer {
	uint8_t buf[DISP_H][DISP_W];
	bool dirty;
	uint16_t x1, x2, y1, y2; // partial update bounds
};

/**
 * Fill the framebuffer with a pattern in the specified rectangular region.
 */
void fb_fill(struct framebuffer *const fb, uint8_t pattern, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * fb - framebuffer object
 * src - subsequent columns of sprite pixels
 * mask - mask to blit through
 * x1 - pixel x to start at
 * y1 - byte row y to start at
 * x2 - pixel x to stop at
 * y2 - byte row to stop at
 * 
 * blitted glyph will be (x2-x1) bits wide and (y2-y1) bytes tall
 *
 * returns ending x coord
 */
uint16_t fb_blit(struct framebuffer *const fb, const uint8_t *src, const uint8_t *mask, uint16_t x, uint16_t y, uint16_t w, uint16_t h);

/**
 * Blit a text string starting at coords x and y from the top left. \n is
 * interpreted as a carriage return and text is wrapped at the screen edge.
 *
 * Returns the ending x coordinate of the cursor.
 */
uint16_t fb_draw_font(struct framebuffer *const fb, const struct font* font, uint16_t x, uint16_t y, const char *str);

/**
 * Reset dirty markers
 */
void fb_clean(struct framebuffer *const fb);

/**
 * Fill fb with 0x00
 */
void fb_clear(struct framebuffer *const fb);

/**
 * TODO:
 * - Arbitrary pixel row alignment
 * - Blit using Porter-Duff ([Src in Mask] or Over)
 * - Abstract out some app-specific parameters (frame size etc.)
 */

#endif
