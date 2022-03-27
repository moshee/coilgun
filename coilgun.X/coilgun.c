#include <stdlib.h>
#include <stdint.h>

#include "coilgun.h"
#include "plib_clock.h"
#include "font.h"

cg_prog_t prog = {0};

// cg_sm_t sm_main, sm_display;

cg_sm_t *machines[] = {
    &prog.sm_main,
    &prog.sm_display
};

void cg_init(cg_prog_t *prog) {
    // digital I/O
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_CHARGE
                                    | PIN_DISP_nRES
                                    | PIN_DISP_D_nC
                                    | PIN_MOSI
                                    | PIN_SCK
                                    | PIN_nCS
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

    // enable digital inputs
    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_HWSEL_Msk
                                      | PORT_WRCONFIG_WRPINCFG_Msk
                                      | PORT_PINCFG_INEN_Msk
                                      | BIT(17 - 16) // DONE
                                      | BIT(18 - 16) // FAULT
                                      | BIT(21 - 16) // OPTO
                                      | BIT(24 - 16) // MOSI
                                      ;

    // set up EIC for control inputs
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

    // SERCOM3 used for display SPI

    PORT_REGS->GROUP[0].PORT_WRCONFIG = PORT_WRCONFIG_HWSEL_Msk
                                      | PORT_WRCONFIG_WRPINCFG_Msk
                                      | PORT_WRCONFIG_WRPMUX_Msk
                                      | PORT_WRCONFIG_PMUX(PORT_PMUX_PMUXE_C)
                                      | PORT_WRCONFIG_PMUXEN_Msk
                                      | BIT(22 - 16) // MISO
                                      | BIT(23 - 16) // nCS
                                      | BIT(24 - 16) // MOSI
                                      | BIT(25 - 16) // SCK
                                      ;


    // SERCOM3_REGS->SPIM.SERCOM_INTENSET = SERCOM_SPIM_INTENSET_TXC_Msk
    //                                    | SERCOM_SPIM_INTENSET_DRE_Msk
    //                                    ;

    // SSD1305 is 4 MHz (250 ns) max
    // BAUD = 48 MHz / (2 * 4MHz) - 1
    SERCOM3_REGS->SPIM.SERCOM_BAUD = 5;

    SERCOM3_REGS->SPIM.SERCOM_CTRLB = SERCOM_SPIM_CTRLB_MSSEN_Msk;

    // CPOL = 0, CPHA = 0
    SERCOM3_REGS->SPIM.SERCOM_CTRLA = SERCOM_SPIM_CTRLA_MODE_SPI_MASTER
                                    | SERCOM_SPIM_CTRLA_CPOL_IDLE_LOW
                                    | SERCOM_SPIM_CTRLA_CPHA_LEADING_EDGE
                                    | SERCOM_SPIM_CTRLA_DIPO(0x0)
                                    | SERCOM_SPIM_CTRLA_DOPO(0x1)
                                    | SERCOM_SPIM_CTRLA_ENABLE_Msk
                                    ;
    WAIT_UNTIL_CLEAR(SERCOM3_REGS->SPIM.SERCOM_SYNCBUSY,
        SERCOM_SPIM_SYNCBUSY_ENABLE_Msk | SERCOM_SPIM_SYNCBUSY_CTRLB_Msk);

    // ADC setup

    // External interrupt setup
    EIC_REGS->EIC_CTRLA = EIC_CTRLA_SWRST_Msk;
    WAIT_UNTIL_CLEAR(EIC_REGS->EIC_SYNCBUSY, EIC_SYNCBUSY_ENABLE_Msk | EIC_SYNCBUSY_SWRST_Msk);

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
    WAIT_UNTIL_CLEAR(EIC_REGS->EIC_SYNCBUSY, EIC_SYNCBUSY_ENABLE_Msk);

    cg_sm_init(&prog->sm_main, state_idle, MSG_CONTROL);
    cg_sm_init(&prog->sm_display, state_display, MSG_FRAME);

    prog->super.machines = machines;
    prog->super.len = sizeof(machines)/sizeof(machines[0]);

    cg_timer_init(&prog->blinker, 250, timer_blink);
    cg_timer_init(&prog->frame, 33, timer_frame);

    uint32_t portb = port_read(PORTB);
    prog->q.a = (portb & PIN_ENC_A) != 0;
    prog->q.b = (portb & PIN_ENC_B) != 0;

    prog->blinker.enable = 1;
    prog->frame.enable = 1;

    prog->display.port_reset = PORTA;
    prog->display.pin_reset = PIN_DISP_nRES;
    prog->display.port_dc = PORTA;
    prog->display.pin_dc = PIN_DISP_D_nC;
    prog->display.sercom = SERCOM3_REGS;
    ssd1305_init(&prog->display);

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
    SM_DISPATCH_EMPTY(&prog, MSG_FRAME);
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
            ret = SM_NEXT(sm, state_armed);
            break;
        case MSG_CONTROL_CHG_ON:
            ret = SM_NEXT(sm, state_charging);
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
            ret = SM_NEXT(sm, state_idle);
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
            ret = SM_NEXT(sm, state_idle);
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

struct state_str {
    cg_state_t fn;
    const char *str;
};

static const struct state_str state_str_map[] = {
    {state_idle, "IDLE"},
    {state_armed, "ARMED"},
    {state_charging, "CHARGING"},
    {state_charged, "CHARGED"},
    {state_ready_to_fire, "READY"},
    {state_firing, "FIRING"}
};

STATE(state_display, sm, msg) {
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

    case MSG_FRAME: {
        // gather state from prog
        // build frame
        // send frame to display

        int i;
        const struct state_str *kv;
        const char *str = "<?>";

        for (i = 0; i < LEN(state_str_map); i++) {
            kv = &state_str_map[i];
            if (kv->fn == prog.sm_main.state) {
                str = kv->str;
                break;
            }
        }

        // print str

        fb_fill(&prog.fb, 0x00, 0, 0, 128, 1);
        fb_draw_font(&prog.fb, &font_5x7, 0, 0, str);
        
        if (prog.fb.dirty) {
            ssd1305_cmd3(&prog.display, SSD1305_CMD_COL_ADDR, prog.fb.x1, prog.fb.x2);
            ssd1305_cmd3(&prog.display, SSD1305_CMD_PAGE_ADDR, prog.fb.y1, prog.fb.y2);
            int dx = prog.fb.x2 - prog.fb.x1;
            for (int y = prog.fb.y1; y < prog.fb.y2; y++) {
                ssd1305_data(&prog.display, &prog.fb.buf[y][prog.fb.x1], dx);
            }
            fb_clean(&prog.fb);
        }

        ret = YIELD;
    }

    default:
        ret = YIELD;
    }

    return ret;
}