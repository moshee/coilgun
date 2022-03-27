/* 
 * File:   driver.h
 * Author: moshee
 *
 * Created on January 29, 2022, 1:41 PM
 */

#ifndef DRIVER_H
#define	DRIVER_H

#ifdef	__cplusplus
extern "C" {
#endif

#include "samc20j16a.h"

#define BIT(x) (1 << (x))
#define WAIT_UNTIL_CLEAR(_reg, _msk) while ((_reg) & (_msk))
#define WAIT_UNTIL_SET(_reg, _msk) while (((_reg) & (_msk)) != (_msk))


// port_group_registers_t *PORTA, *PORTB;

#define PORTA 0
#define PORTB 1

void driver_init(void);
void port_set(const uint8_t port, const uint32_t val);
void port_write(const uint8_t port, const uint32_t val);
void port_clear(const uint8_t port, const uint32_t val);
void port_toggle(const uint8_t port, const uint32_t val);
uint32_t port_read(const uint8_t port);
void port_dirset(const uint8_t port, const uint32_t val);
void port_dirwrite(const uint8_t port, const uint32_t val);
void port_dirclear(const uint8_t port, const uint32_t val);
void port_dirtoggle(const uint8_t port, const uint32_t val);

struct spi_settings {
    sercom_registers_t *SERCOM;
    uint32_t CTRLA, CTRLB, BAUD, INTEN;
};

// blocking spi write
void spi_write(volatile sercom_registers_t *const sercom, const uint8_t *buf, const int len);
// blocking spi write of a single byte
void spi_write_byte(volatile sercom_registers_t *const sercom, const uint8_t b);
void spi_repeat_byte(volatile sercom_registers_t *const sercom, const uint8_t b, const uint32_t cnt);
void spi_init_settings(struct spi_settings *const set, const uint32_t baud);

void delay_us(const uint32_t us);
void delay_ms(const uint32_t ms);

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_H */

