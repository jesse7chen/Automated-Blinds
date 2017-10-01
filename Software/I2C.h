#ifndef _I2C_H_
#define _I2C_H_

#include "LPC11xx.h"

void I2C_init(void);
static uint8_t write_addr(uint8_t addr, uint8_t r_w);
static uint8_t write_byte(uint8_t data);
static void read_byte(uint8_t* data, uint8_t remaining_bytes);
uint8_t I2C_write(uint8_t addr, uint8_t *data, uint32_t length);
uint8_t I2C_read(uint8_t addr, uint8_t *data, uint32_t length);





#endif /* _I2C_H_ */
