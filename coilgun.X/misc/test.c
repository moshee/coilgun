#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <errno.h>

#include "../graphics.h"
#include "../font.h"

int main(void) {
    FILE *f = fopen("test.pbm", "w");
    if (f == NULL) {
        printf("failed to open file: %d", errno);
        return 1;
    }

    fprintf(f, "P1\n128 64\n");

    struct framebuffer fb = {{{0}}};
    printf("1\n");
    fb_draw_font(&fb, &font_5x7, 0, 0, "Hello,\nworld!");
    printf("2\n");

    int x, y, i;
    uint8_t ch;

    for (y = 0; y < DISP_H; y++) {
        for (i = 0; i < 8; i++) {
            for (x = 0; x < DISP_W; x++) {
                ch = fb.buf[y][x];
                ch = (ch >> i) & 0x01;
                if ((ch & 0x01) != 0) {
                    fprintf(f, "0 ");
                } else {
                    fprintf(f, "1 ");
                }
            }
        }
        fprintf(f, "\n");
    }

    printf("3\n");
    
    fclose(f);

    return 0;
}