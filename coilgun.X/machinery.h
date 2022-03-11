/* 
 * File:   machinery.h
 * Author: moshee
 *
 * Created on March 2, 2022, 9:00 PM
 */

#ifndef MACHINERY_H
#define	MACHINERY_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdlib.h>

#include "samc20j16a.h"

/**
 * SysTick-powered timer
 */

typedef struct cg_timer {
    uint32_t _cnt;
    uint32_t reload;
    uint8_t enable;
    void (*callback)(void);
} cg_timer_t;

void cg_timer_init(cg_timer_t *t, uint32_t duration, void (*callback)(void));
void cg_timer_tick(cg_timer_t *t);

/**
 * Message queue
 * Inspiration: downtowndougbrown.com/2013/01/microcontrollers-interrupt-safe-ring-buffers/
 */

typedef uint32_t mq_msgid_t;

typedef union mq_data {
    uint32_t n;
    void *ptr;
} mq_data_t;

typedef struct mq_msg {
    mq_msgid_t id;
    mq_data_t data;
} mq_msg_t;

#define MQ_SIZE 16

typedef uint32_t mq_pos_t;

typedef struct mq {
    mq_msg_t buf[MQ_SIZE];
    volatile mq_pos_t head; // pointer to newest object
    volatile mq_pos_t tail; // pointer to oldest object
    mq_msgid_t accept; // bitmask of accepted message IDs
} mq_t;

void mq_init(mq_t *mq, mq_msgid_t accept_ids);
int mq_push(mq_t *mq, mq_msgid_t id, mq_data_t data);
int mq_push_empty(mq_t *mq, mq_msgid_t id);
mq_msg_t *mq_pop(mq_t *mq);

/**
 * State machine. Loosely based around the Quantum Leaps "optimal" C design:
 * https://github.com/QuantumLeaps/modern-embedded-programming-course/tree/master/lesson-39
 */

#define MSG_STATE BIT(0) // Special event used for state machine functions.

/**
 * Special state machine event values.
 */
enum MSG_STATE_Val {
    MSG_STATE_ENTRY, // Passed to the state function upon transition into the state.
    MSG_STATE_EXIT // Passed to the state function upon transition out of the state.
};

typedef struct cg_sm cg_sm_t;

/**
 * State function return codes, intended to instruct the state machine on the
 * immediate action to take.
 * 
 * TODO: This stuff might not be needed, or something else would be appropriate.
 */
typedef enum cg_sm_ret {
    NEXT, // Immediately advance to the next state with the same message.
    YIELD // Finish the execution cycle and wait for the next message.
} cg_sm_ret_t;

typedef cg_sm_ret_t (*cg_state_t)(cg_sm_t * const, const mq_msg_t * const);

struct cg_sm {
    cg_state_t state; // The current state of the machine.
    mq_t mq; // Message queue for this machine.
};

/**
 * Initialize the state machine.
 * sm: the state machine
 * initial: the initial state of the machine
 * sub: bitmask of the subscribed message IDs of this state. It will only
 *   receive messages on the sub list.
 */
void cg_sm_init(cg_sm_t *const sm, const cg_state_t initial, const mq_msgid_t sub);

/**
 * Advance the state machine, checking for any messages and passing them along
 * to the state function if it matches the sub list.
 */
void cg_sm_crank(cg_sm_t *sm);

/**
 * Transition to the next state, passing the exit event to the current one and
 * the entry state to the next.
 */
cg_sm_ret_t cg_sm_next(cg_sm_t *const sm, const cg_state_t next);

typedef struct quadrature {
    uint8_t a;
    uint8_t b;
} quadrature_t;

/**
 * Update the quadrature decoder and return 1 (moved forward), -1 (moved
 * backward), or 0 (no move or illegal sequence)
 */
int quadrature_update(quadrature_t *q, uint8_t a, uint8_t b);

#ifdef	__cplusplus
}
#endif

#endif	/* MACHINERY_H */

