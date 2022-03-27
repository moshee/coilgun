#include <string.h>

#include "machinery.h"
#include "driver.h"

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

void mq_init(mq_t *mq, const mq_msgid_t accept_ids) {
    memset(mq->buf, 0, sizeof mq->buf);
    mq->head = 0;
    mq->tail = 0;
    mq->accept = accept_ids;
}

int mq_push(mq_t *mq, const mq_msgid_t id, const mq_data_t data) {
    if ((mq->accept & id) != id) {
        return 1;
    }

    mq_pos_t next = (mq->head + 1) % MQ_SIZE;

    if (next != mq->tail) {
        mq_msg_t *msg = &mq->buf[mq->head];
        msg->id = id;
        msg->data = data;
        mq->head = next;

        return 0;
    }

    return -1;
}

mq_msg_t *mq_pop(mq_t *mq) {
    if (mq->tail != mq->head) {
        mq_msg_t *ret = &mq->buf[mq->tail];
        mq->tail = (mq->tail + 1) % MQ_SIZE;
        return ret;
    }

    return NULL;
}

void cg_sm_init(cg_sm_t *const sm, const cg_state_t initial, const mq_msgid_t sub) {
    sm->state = initial;
    mq_init(&sm->mq, sub);
}

void cg_sm_dispatch(cg_app_t *const app, const mq_msgid_t id, const mq_data_t data) {
    cg_sm_t *sm;

    if (app->len == 0) {
        return;
    }

    for (int i = 0; i < app->len; i++) {
        sm = app->machines[i];
        mq_push(&sm->mq, id, data);
    }
}

void cg_sm_dispatch_empty(cg_app_t *const app, const mq_msgid_t id) {
    mq_data_t data = {0};

    cg_sm_dispatch(app, id, data);
}


void cg_sm_crank(cg_app_t *const app) {
    cg_sm_ret_t ret;
    mq_msg_t *msg;
    cg_sm_t *sm;

    for (int i = 0; i < app->len; i++) {
        sm = app->machines[i];
        msg = mq_pop(&sm->mq);
        if (msg == NULL) {
            return;
        }
        for (ret = NEXT; ret != YIELD; ret = sm->state(sm, msg));
    }
    
}

cg_sm_ret_t cg_sm_next(cg_sm_t *const sm, const cg_state_t next) {
    mq_msg_t msg = {
        .id = MSG_STATE,
        .data.n = MSG_STATE_EXIT
    };
    sm->state(sm, &msg);
    sm->state = next;
    msg.data.n = MSG_STATE_ENTRY;
    sm->state(sm, &msg);

    return YIELD;
}

int quadrature_update(quadrature_t *q, uint8_t a, uint8_t b) {
    uint8_t old_a = q->a;
    uint8_t old_b = q->b;
    uint8_t val = (old_a << 3) | (old_b << 2) | (a << 1) | (b << 0);

    q->a = a;
    q->b = b;

    /**
     * CW:
     *       ___     ___
     * A ___/   \___/
     *   0 0 1 1 0 0 1 1
     *   1 0 0 1 1 0 0 1
     *   _     ___     _
     * B  \___/   \___/
     * 
     * 
     * A' | B' | A | B | OUT
     * 0  | 0  | 0 | 0 |  X
     * 0  | 0  | 0 | 1 | -1
     * 0  | 0  | 1 | 0 |  1
     * 0  | 0  | 1 | 1 |  X
     * 0  | 1  | 0 | 0 |  1
     * 0  | 1  | 0 | 1 |  0
     * 0  | 1  | 1 | 0 |  X
     * 0  | 1  | 1 | 1 | -1
     * 1  | 0  | 0 | 0 | -1
     * 1  | 0  | 0 | 1 |  X
     * 1  | 0  | 1 | 0 |  0
     * 1  | 0  | 1 | 1 |  1
     * 1  | 1  | 0 | 0 |  X
     * 1  | 1  | 0 | 1 |  1
     * 1  | 1  | 1 | 0 | -1
     * 1  | 1  | 1 | 1 |  0
     */

    switch (val & 0x0F) {
    case 0x02:
    case 0x04:
    case 0x0B:
    case 0x0D:
        return 1;
    case 0x01:
    case 0x07:
    case 0x08:
    case 0x0E:
        return -1;
    }

    return 0;
}