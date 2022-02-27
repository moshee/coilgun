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

port_group_registers_t *PORTA, *PORTB;

void driver_init(void);

#ifdef	__cplusplus
}
#endif

#endif	/* DRIVER_H */

