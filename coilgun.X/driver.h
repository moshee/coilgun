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

// port_group_registers_t *PORTA, *PORTB;

#define PORTA 0
#define PORTB 1

void driver_init(void);
void port_set(const uint32_t port, uint32_t val);
void port_write(const uint32_t port, uint32_t val);
void port_clear(const uint32_t port, uint32_t val);
void port_toggle(const uint32_t port, uint32_t val);
uint32_t port_read(const uint32_t port);
void port_dirset(const uint32_t port, uint32_t val);
void port_dirwrite(const uint32_t port, uint32_t val);
void port_dirclear(const uint32_t port, uint32_t val);
void port_dirtoggle(const uint32_t port, uint32_t val);

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_H */

