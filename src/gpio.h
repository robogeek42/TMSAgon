#ifndef GPIO_H
#define GPIO_H

#include <stdio.h>
#include <stdint.h>
#include <ez80f92.h>
#include <agon/vdp.h>

#define PB_DAT 0x009A
#define PB_DIR 0x009B
#define PC_DAT 0x009E
#define PC_DIR 0x009F
#define PD_DAT 0x00A2
#define PD_DIR 0x00A3

uint8_t io_in(int addr);
void io_out(int addr, uint8_t value);

#endif
