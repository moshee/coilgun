#include <stdlib.h>
#include <stdint.h>

#include "coilgun.h"

void cg_timer_init(cg_timer_t *t, uint32_t duration, void (*callback)(void)) {
    t->_cnt = 0;
    t->reload = duration;
    t->callback = callback;
}

void cg_timer_tick(cg_timer_t *t) {
    if (!t->enable) {
        return;
    }
    t->_cnt = (t->_cnt + 1) % t->reload;
    if (t->_cnt == 0 && t->callback != NULL) {
        t->callback();
    }
}

void cg_timer_enable(cg_timer_t *t, uint8_t en) {
    t->enable = en;
}