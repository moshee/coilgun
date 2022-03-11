#include "driver.h"

void driver_init(void) {
    // PORTA = &(PORT_REGS->GROUP[0]);
    // PORTB = &(PORT_REGS->GROUP[1]);
}

void port_set(const uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTSET = val;
}

void port_write(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUT = val;
}

void port_clear(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTCLR = val;
}

void port_toggle(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTTGL = val;
}

uint32_t port_read(uint32_t port) {
    return PORT_REGS->GROUP[port].PORT_IN;
}

void port_dirset(const uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_DIRSET = val;
}

void port_dirwrite(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_DIR = val;
}

void port_dirclear(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTCLR = val;
}

void port_dirtoggle(uint32_t port, uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTTGL = val;
}