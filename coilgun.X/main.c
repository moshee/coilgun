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
#include "font.h"
#include "coilgun.h"

int main(void) {
    WDT_REGS->WDT_CTRLA = 0;

    CLOCK_Initialize();
    SysTick_Config(48000);
    driver_init();

    cg_init(&prog);

    for (;;) {
        cg_sm_crank((cg_app_t *)&prog);
    }

    return (EXIT_SUCCESS);
}

void SysTick_Handler(void) {
    mq_data_t data = {0};

    SM_DISPATCH(&prog, MSG_TICK, data);

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
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        } else {
            // ENC_SW released
            data.n = MSG_CONTROL_ENC_RELEASE;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW0) {
        if (in & PIN_SW0) {
            // SW off
            data.n = MSG_CONTROL_ARM_OFF;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_ARM_ON;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW1) {
        if (in & PIN_SW1) {
            // SW off
            data.n = MSG_CONTROL_CHG_OFF;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_CHG_ON;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        }
    }
    if (intflag & PIN_SW2) {
        if (in & PIN_SW2) {
            // SW off
            data.n = MSG_CONTROL_FIRE_RELEASE;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
        } else {
            // SW on
            data.n = MSG_CONTROL_FIRE_PRESS;
            SM_DISPATCH(&prog, MSG_CONTROL, data);
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