#ifndef _COILGUN_H_
#define _COILGUN_H_

#include "samc20j16a.h"
#include "machinery.h"
#include "driver.h"
#include "ssd1305.h"
#include "graphics.h"

#define PIN_TXD PORT_PA00
#define PIN_RXD PORT_PA01
#define PIN_ST1_VOLTAGE PORT_PA02
#define PIN_ST1_CURRENT PORT_PA03
#define PIN_ENC_SW PORT_PB05
#define PIN_ENC_A PORT_PB06
#define PIN_ENC_B PORT_PB07
#define PIN_CHG_VOLTAGE PORT_PB08
#define PIN_CHG_CURRENT PORT_PB09
#define PIN_ST2_VOLTAGE PORT_PA04
#define PIN_ST2_CURRENT PORT_PA05
#define PIN_ST3_VOLTAGE PORT_PA06
#define PIN_ST3_CURRENT PORT_PA07
#define PIN_ST4_VOLTAGE PORT_PA08
#define PIN_ST4_CURRENT PORT_PA09
#define PIN_AIN10 PORT_PA10
#define PIN_AIN11 PORT_PA11
#define PIN_LED0 PORT_PB10
#define PIN_LED1 PORT_PB11
#define PIN_LED2 PORT_PB12
#define PIN_SW0 PORT_PB13
#define PIN_SW1 PORT_PB14
#define PIN_SW2 PORT_PB15
#define PIN_SDA PORT_PA12
#define PIN_SCL PORT_PA13
#define PIN_XIN PORT_PA14
#define PIN_XOUT PORT_PA15
#define PIN_CHARGE PORT_PA16
#define PIN_DONE PORT_PA17
#define PIN_FAULT PORT_PA18
#define PIN_ARM PORT_PA19
#define PIN_SEL0 PORT_PB16
#define PIN_SEL1 PORT_PB17
#define PIN_OPTO PORT_PA21
#define PIN_MISO PORT_PA22
#define PIN_nCS PORT_PA23
#define PIN_MOSI PORT_PA24
#define PIN_SCK PORT_PA25
#define PIN_OUTL PORT_PB22
#define PIN_OUTH PORT_PB23
#define PIN_DISP_nRES PORT_PA27
#define PIN_DISP_D_nC PORT_PA28
#define PIN_VEL_P PORT_PB30
#define PIN_VEL_N PORT_PB31
#define PIN_INJ_ENBL PORT_PB02

typedef struct cg_prog {
    cg_app_t super;

    cg_timer_t blinker;
    uint32_t blink_mask;

    cg_timer_t frame;

    quadrature_t q;

    cg_sm_t sm_main;
    cg_sm_t sm_display;

    ssd1305_t display;
    struct framebuffer fb;

    uint32_t eic_events;
} cg_prog_t;

extern cg_prog_t prog;

void cg_init(cg_prog_t *prog);

/**
 * Message ID definitions. They should be defined as bit masks so that they can
 * be ORed together to denote state machine subscriptions.
 */
#define MSG_TICK BIT(1)
#define MSG_CONTROL BIT(2)
#define MSG_TCNT BIT(3)
#define MSG_FRAME BIT(4)

enum MSG_CONTROL_Val {
    MSG_CONTROL_ARM_ON,
    MSG_CONTROL_ARM_OFF,
    MSG_CONTROL_CHG_ON,
    MSG_CONTROL_CHG_OFF,
    MSG_CONTROL_FIRE_PRESS,
    MSG_CONTROL_FIRE_RELEASE,
    MSG_CONTROL_ENC_PRESS,
    MSG_CONTROL_ENC_RELEASE,
    MSG_CONTROL_ENC_UP,
    MSG_CONTROL_ENC_DOWN
};

STATE(state_idle, sm, msg);
STATE(state_armed, sm, msg);
STATE(state_charging, sm, msg);
STATE(state_charged, sm, msg);
STATE(state_ready_to_fire, sm, msg);
STATE(state_firing, sm, msg);
STATE(state_menu, sm, msg);

STATE(state_display, sm, msg);

void timer_blink(void);
void blink_set(uint32_t msk);
void blink_clr(uint32_t msk);

void timer_frame(void);

#endif
