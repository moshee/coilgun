#ifdef __PC_TEST__
#include <stdio.h>
#endif

#include <stdlib.h>

#include "graphics.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) >= (y) ? (x) : (y))

void fb_fill(struct framebuffer *const fb, uint8_t pattern, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	uint16_t _x, _y;
	const uint16_t y_bottom = y;
	const uint16_t y_top = y + h;

	for (_y = y_bottom; _y < y_top; _y++) {
		for (_x = x; _x < (x+w); _x++) {
			fb->buf[_y][_x] = pattern;
		}
	}

	fb->dirty = true;
	fb->x1 = MIN(fb->x1, x);
	fb->x2 = MAX(fb->x2, x + w);
	fb->y1 = MIN(fb->y1, y);
	fb->y2 = MAX(fb->y2, y_top);
}

uint16_t fb_blit(struct framebuffer *const fb, const uint8_t *src, const uint8_t *mask, uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
	uint16_t _x = x, _y = y;
	uint16_t x2 = x + w;
	uint16_t y2 = y + h;

	if (x2 > (DISP_W - 1)) {
		x2 = DISP_W - 1;
	}

	if (y2 > (DISP_H - 1)) {
		y2 = DISP_H - 1;
	}

	for (_y = y; _y < y2; _y++) {
		for (_x = x; _x < x2; _x++) {
			fb->buf[_y][_x] = *src++;
		}
	}

	fb->dirty = true;
	fb->x1 = MIN(fb->x1, x);
	fb->x2 = MAX(fb->x2, x2);
	fb->y1 = MIN(fb->y1, y);
	fb->y2 = MAX(fb->y2, y2);

	return _x;
}

uint16_t fb_draw_font(struct framebuffer *const fb, const struct font* font, uint16_t x, uint16_t y, const char *str) {
	char c;
	const uint8_t* glyph;
	uint16_t cursor_x = x;

#ifdef __PC_TEST__
	printf("1.1\n");
#endif

	while ((c = *str++)) {
		if (c == '\n') {
			cursor_x = x;
			y++;
			continue;
		}
		glyph = font_lookup(font, c);
#ifdef __PC_TEST__
		printf("0x%02hhX 0x%02hhX 0x%02hhX 0x%02hhX 0x%02hhX 0x%02hhX 0x%02hhX\n",
				glyph[0], glyph[1], glyph[2], glyph[3], glyph[4], glyph[5], glyph[6]);
#endif
		(void)fb_blit(fb, glyph, font->mask, cursor_x, y, font->width, 1);
		cursor_x += (font->width);
	}

	return cursor_x + 1;
}

void fb_clean(struct framebuffer *const fb) {
	fb->dirty = 0;
	fb->x1 = 0;
	fb->x2 = 0;
	fb->y1 = 0;
	fb->y2 = 0;
}

void fb_clear(struct framebuffer *const fb) {
	fb_fill(fb, 0x00, 0, 0, DISP_W, DISP_H);
}