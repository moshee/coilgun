#include <stdlib.h>
#include <stdint.h>

#include "coilgun.h"
#include "plib_clock.h"

cg_prog_t prog = {0};

cg_sm_t sm_main, sm_display;

void cg_init(cg_prog_t *prog) {
    WDT_REGS->WDT_CTRLA = 0;

    CLOCK_Initialize();
    SysTick_Config(48000);

    // digital I/O
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_CHARGE
                                    | PIN_DISP_nRES
                                    | PIN_DISP_D_nC
                                    ;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_LED0
                                    | PIN_LED1
                                    | PIN_LED2
                                    | PIN_SEL0
                                    | PIN_SEL1
                                    | PIN_OUTL
                                    | PIN_OUTH
                                    | PIN_INJ_ENBL
                                    ;

    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_HWSEL_Msk
                                      | PORT_WRCONFIG_WRPINCFG_Msk
                                      | PORT_PINCFG_INEN_Msk
                                      | BIT(17 - 16)
                                      | BIT(18 - 16)
                                      | BIT(21 - 16)
                                      ;

    PORT_REGS->GROUP[1].PORT_WRCONFIG = PORT_WRCONFIG_WRPINCFG_Msk
                                      | PORT_WRCONFIG_WRPMUX_Msk
                                      | PORT_WRCONFIG_PMUX(PORT_PMUX_PMUXE_A)
                                      | PORT_WRCONFIG_PMUXEN_Msk
                                      | PORT_WRCONFIG_INEN_Msk
                                      | BIT(5) // ENC_SW
                                      | BIT(6) // ENC_A
                                      | BIT(7) // ENC_B
                                      | BIT(13) // SW0
                                      | BIT(14) // SW1
                                      | BIT(15) // SW2
                                      ;

    // ADC setup

    // External interrupt setup
    EIC_REGS->EIC_CTRLA = EIC_CTRLA_SWRST_Msk;
    while (EIC_REGS->EIC_SYNCBUSY & (EIC_SYNCBUSY_ENABLE_Msk | EIC_SYNCBUSY_SWRST_Msk));

    EIC_REGS->EIC_CONFIG[0] = EIC_CONFIG_FILTEN5_Msk
                            | EIC_CONFIG_SENSE5_BOTH
                            | EIC_CONFIG_FILTEN6_Msk
                            | EIC_CONFIG_SENSE6_BOTH
                            | EIC_CONFIG_FILTEN7_Msk
                            | EIC_CONFIG_SENSE7_BOTH
                            ;
    EIC_REGS->EIC_CONFIG[1] = EIC_CONFIG_FILTEN5_Msk
                            | EIC_CONFIG_SENSE5_BOTH
                            | EIC_CONFIG_FILTEN6_Msk
                            | EIC_CONFIG_SENSE6_BOTH
                            | EIC_CONFIG_FILTEN7_Msk
                            | EIC_CONFIG_SENSE7_BOTH
                            ;
    EIC_REGS->EIC_INTENSET = BIT(5) // ENC_SW/PB5
                           | BIT(6) // ENC_A/PB6
                           | BIT(7) // ENC_B/PB7
                           | BIT(13) // SW0/PB13
                           | BIT(14) // SW1/PB14
                           | BIT(15) // SW2/PB15
                           ;

    EIC_REGS->EIC_CTRLA = EIC_CTRLA_ENABLE_Msk;
    while (EIC_REGS->EIC_SYNCBUSY & EIC_SYNCBUSY_ENABLE_Msk);

    cg_sm_init(&sm_main, state_idle, MSG_CONTROL);

    cg_sm_t machines[] = {
        sm_main,
        sm_display
    };

    prog->app.machines = machines;
    prog->app.len = sizeof(machines)/sizeof(machines[0]);

    cg_timer_init(&prog->blinker, 250, timer_blink);
    cg_timer_init(&prog->frame, 33, timer_frame);

    uint32_t portb = port_read(PORTB);
    prog->q.a = (portb & PIN_ENC_A) != 0;
    prog->q.b = (portb & PIN_ENC_B) != 0;

    prog->blinker.enable = 1;
    prog->frame.enable = 1;

    // Enable interrupt vectors
    NVIC_EnableIRQ(SysTick_IRQn);
    NVIC_EnableIRQ(EIC_IRQn);
}

#define LED_Msk (PIN_LED0 | PIN_LED1 | PIN_LED2)

void timer_blink(void) {
    port_toggle(PORTB, prog.blink_mask & LED_Msk);
    port_clear(PORTB, (~prog.blink_mask) & LED_Msk);
}

void blink_set(uint32_t msk) {
    prog.blink_mask |= msk & LED_Msk;
}

void blink_clear(uint32_t msk) {
    prog.blink_mask &= ~(msk & LED_Msk);
    port_clear(PORTB, msk & LED_Msk);
}

void timer_frame(void) {
    cg_sm_dispatch_empty(&prog.app, MSG_FRAME);
}

/**
 * State functions
 */


STATE(state_idle, sm, msg) {
    cg_sm_ret_t ret;

    switch (msg->id) {
    case MSG_STATE:
        switch (msg->data.n) {
        case MSG_STATE_ENTRY:
            ret = YIELD;
            break;
        case MSG_STATE_EXIT:
            ret = YIELD;
            break;
        default:
            ret = YIELD;
            break;
        }
        break;

    case MSG_CONTROL:
        switch (msg->data.n) {
        case MSG_CONTROL_ARM_ON:
            ret = cg_sm_next(sm, state_armed);
            break;
        case MSG_CONTROL_CHG_ON:
            ret = cg_sm_next(sm, state_charging);
            break;
        default:
            ret = YIELD;
        }
        break;

    default:
        ret = YIELD;
    }

    return ret;
}

STATE(state_armed, sm, msg) {
    cg_sm_ret_t ret;

    switch (msg->id) {
    case MSG_STATE:
        switch (msg->data.n) {
        case MSG_STATE_ENTRY:
            blink_set(PIN_LED0);
            ret = YIELD;
            break;
        case MSG_STATE_EXIT:
            blink_clear(PIN_LED0);
            ret = YIELD;
            break;
        default:
            ret = YIELD;
            break;
        }
        break;

    case MSG_CONTROL:
        switch (msg->data.n) {
        case MSG_CONTROL_ARM_OFF:
            ret = cg_sm_next(sm, state_idle);
            break;
        default:
            ret = YIELD;
        }
        break;

    default:
        ret = YIELD;
        break;
    }

    return ret;
}

STATE(state_charging, sm, msg) {
    cg_sm_ret_t ret;

    switch (msg->id) {
    case MSG_STATE:
        switch (msg->data.n) {
        case MSG_STATE_ENTRY:
            blink_set(PIN_LED1);
            ret = YIELD;
            break;
        case MSG_STATE_EXIT:
            blink_clear(PIN_LED1);
            ret = YIELD;
            break;
        default:
            ret = YIELD;
            break;
        }
        break;

    case MSG_CONTROL:
        switch (msg->data.n) {
        case MSG_CONTROL_CHG_OFF:
            ret = cg_sm_next(sm, state_idle);
            break;
        default:
            ret = YIELD;
        }
        break;

    default:
        ret = YIELD;
        break;
    }

    return ret;
}

STATE(state_charged, sm, msg) {
    return YIELD;
}

STATE(state_ready_to_fire, sm, msg) {
    return YIELD;
}

STATE(state_firing, sm, msg) {
    return YIELD;
}