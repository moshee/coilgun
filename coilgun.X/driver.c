#include "driver.h"

void driver_init(void) {
    PORTA = &(PORT_REGS->GROUP[0]);
    PORTB = &(PORT_REGS->GROUP[1]);
}