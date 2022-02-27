#ifndef _COILGUN_H_
#define _COILGUN_H_

#include <stdint.h>

#include "samc20j16a.h"

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

// State machine state definition.
typedef enum state {
    STATE_IDLE,
    STATE_ARMED,
    STATE_FIRING,
    STATE_CHARGING,
    STATE_EDIT
} cg_state_t;

typedef struct cg_timer {
    uint32_t _cnt;
    uint32_t reload;
    uint8_t enable;
    void (*callback)(void);
} cg_timer_t;

void cg_timer_init(cg_timer_t *t, uint32_t duration, void (*callback)(void));
void cg_timer_tick(cg_timer_t *t);

#endif
