/* 
 * File:   main.c
 * Author: moshee
 *
 * Created on January 17, 2022, 9:21 PM
 */

#include <stdio.h>
#include <stdlib.h>

#include "samc20j16a.h"
#include "plib_clock.h"
#include "driver.h"
#include "coilgun.h"

int main(void) {
    cg_init(&prog);

    for (;;) {
        cg_sm_crank(&prog.sm_main);
        cg_sm_crank(&prog.sm_display);
    }

    return (EXIT_SUCCESS);
}

void SysTick_Handler(void) {
    cg_timer_tick(&prog.blinker);
    cg_timer_tick(&prog.frame);
}

void EIC_Handler(void) {
    uint32_t intflag = EIC_REGS->EIC_INTFLAG;
    uint32_t in = PORT_REGS->GROUP[1].PORT_IN;
    mq_data_t data;

    prog.eic_events++;

    if (intflag & PIN_ENC_SW) {
        if (in & PIN_ENC_SW) {
            // ENC_SW pressed
            data.n = MSG_CONTROL_ENC_PRESS;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        } else {
            // ENC_SW released
            data.n = MSG_CONTROL_ENC_RELEASE;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW0) {
        if (in & PIN_SW0) {
            // SW off
            data.n = MSG_CONTROL_SW0_OFF;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_SW0_ON;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW1) {
        if (in & PIN_SW1) {
            // SW off
            data.n = MSG_CONTROL_SW1_OFF;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_SW1_ON;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW2) {
        if (in & PIN_SW2) {
            // SW off
            data.n = MSG_CONTROL_SW2_OFF;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_SW2_ON;
            mq_push(&prog.sm_main.mq, MSG_CONTROL, data);
        }
    }

    if (intflag & (PIN_ENC_A | PIN_ENC_B)) {
        // decode quadrature
        uint8_t a = !!(in & PIN_ENC_A);
        uint8_t b = !!(in & PIN_ENC_B);
        quadrature_update(&prog.q, a, b);
    }

    // clear all interrupt flags
    EIC_REGS->EIC_INTFLAG = intflag;
}