#include "driver.h"
#include "plib_clock.h"

void driver_init(void) {
    // reserve TC0 for general timing/delay functions
    // TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBCLR_ONESHOT_Msk;
    TC0_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_Msk;
    TC0_REGS->COUNT32.TC_CTRLA = TC_CTRLA_SWRST_Msk;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_SWRST_Msk);
    TC0_REGS->COUNT32.TC_WAVE = TC_WAVE_WAVEGEN_MPWM;
    TC0_REGS->COUNT32.TC_CTRLA = TC_CTRLA_MODE_COUNT32 | TC_CTRLA_ENABLE_Msk;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_ENABLE_Msk);
    TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBSET_CMD_STOP;
}

void port_set(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTSET = val;
}

void port_write(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUT = val;
}

void port_clear(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTCLR = val;
}

void port_toggle(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTTGL = val;
}

uint32_t port_read(const uint8_t port) {
    return PORT_REGS->GROUP[port].PORT_IN;
}

void port_dirset(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_DIRSET = val;
}

void port_dirwrite(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_DIR = val;
}

void port_dirclear(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTCLR = val;
}

void port_dirtoggle(const uint8_t port, const uint32_t val) {
    PORT_REGS->GROUP[port].PORT_OUTTGL = val;
}

void spi_write(volatile sercom_registers_t *const sercom, const uint8_t *buf, const int len) {
    int i;
    for (i = 0; i < len; i++) {
        sercom->SPIM.SERCOM_DATA = *buf++;
        WAIT_UNTIL_SET(sercom->SPIM.SERCOM_INTFLAG, SERCOM_SPIM_INTFLAG_DRE_Msk);
    }
    WAIT_UNTIL_SET(sercom->SPIM.SERCOM_INTFLAG, SERCOM_SPIM_INTFLAG_TXC_Msk);
}

void spi_write_byte(volatile sercom_registers_t *const sercom, const uint8_t b) {
    sercom->SPIM.SERCOM_DATA = b;
    WAIT_UNTIL_SET(sercom->SPIM.SERCOM_INTFLAG, SERCOM_SPIM_INTFLAG_TXC_Msk);
}

void spi_repeat_byte(volatile sercom_registers_t *const sercom, const uint8_t b, const uint32_t cnt) {
    int i;
    for (i = 0; i < cnt; i++) {
        sercom->SPIM.SERCOM_DATA = b;
        WAIT_UNTIL_SET(sercom->SPIM.SERCOM_INTFLAG, SERCOM_SPIM_INTFLAG_DRE_Msk);
    }
    WAIT_UNTIL_SET(sercom->SPIM.SERCOM_INTFLAG, SERCOM_SPIM_INTFLAG_TXC_Msk);
}

void spi_init_settings(struct spi_settings *const set, const uint32_t baud) {
    set->BAUD = SYSTEM_CLOCK / (2 * baud) - 1;
}

// unreliable for consistent timing at very short durations (around 3-8 µs of overhead)
void delay_us(const uint32_t us) {
    uint32_t i;

    TC0_REGS->COUNT32.TC_CC[0] = SYSTEM_CLOCK / 1000000;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CC0_Msk);
    // TC0_REGS->COUNT32.TC_COUNT = 0;
    // WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_COUNT_Msk);
    TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBSET_CMD_RETRIGGER;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CTRLB_Msk);

    for (i = 0; i < us; i++) {
        WAIT_UNTIL_SET(TC0_REGS->COUNT32.TC_INTFLAG, TC_INTFLAG_OVF_Msk);
        TC0_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_OVF_Msk;
    }

    TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBSET_CMD_STOP;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CTRLB_Msk);
}

void delay_ms(const uint32_t ms) {
    uint32_t i;

    TC0_REGS->COUNT32.TC_CC[0] = SYSTEM_CLOCK / 1000;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CC0_Msk);
    // TC0_REGS->COUNT32.TC_COUNT = 0;
    // WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_COUNT_Msk);

    // TC0_REGS->COUNT32.TC_CTRLA |= TC_CTRLA_ENABLE_Msk;
    // WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_ENABLE_Msk);

    TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBSET_CMD_RETRIGGER;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CTRLB_Msk);

    for (i = 0; i < ms; i++) {
        WAIT_UNTIL_SET(TC0_REGS->COUNT32.TC_INTFLAG, TC_INTFLAG_OVF_Msk);
        TC0_REGS->COUNT32.TC_INTFLAG = TC_INTFLAG_OVF_Msk;
    }

    // TC0_REGS->COUNT32.TC_CTRLA &= ~TC_CTRLA_ENABLE_Msk;
    // WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_ENABLE_Msk);
    TC0_REGS->COUNT32.TC_CTRLBSET = TC_CTRLBSET_CMD_STOP;
    WAIT_UNTIL_CLEAR(TC0_REGS->COUNT32.TC_SYNCBUSY, TC_SYNCBUSY_CTRLB_Msk);
}