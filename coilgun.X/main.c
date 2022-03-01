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

struct {
    cg_timer_t blinker;
} prog;

void blink(void) {
    PORT_REGS->GROUP[1].PORT_OUTTGL = (PIN_LED1 | PIN_LED0);
}

int main(void) {
    WDT_REGS->WDT_CTRLA = 0;

    CLOCK_Initialize();
    SysTick_Config(48000);

    // SysTick->LOAD = 48000 - 1;
    // SysTick->VAL = 0;
    // SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_ENABLE_Msk;

    cg_timer_init(&prog.blinker, 500, blink);

    // digital I/O
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_LED0;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_LED1;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_LED2;
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_CHARGE;
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_ARM;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_SEL0;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_SEL1;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_OUTL;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_OUTH;
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_DISP_nRES;
    PORT_REGS->GROUP[0].PORT_DIRSET = PIN_DISP_D_nC;
    PORT_REGS->GROUP[1].PORT_DIRSET = PIN_INJ_ENBL;

    // ADC setup

    PORT_REGS->GROUP[1].PORT_OUT |= PIN_LED1;

    NVIC_EnableIRQ(SysTick_IRQn);

    prog.blinker.enable = 1;

    for (;;) {
    }

    return (EXIT_SUCCESS);
}

void SysTick_Handler(void) {
    cg_timer_tick(&prog.blinker);
}