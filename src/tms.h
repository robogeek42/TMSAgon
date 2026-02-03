/*
  vim:ts=4
  vim:sw=4
*/
#ifndef _TMS_H
#define _TMS_H

#include "agon/vdp.h"
#include "agon/mos.h"
#include "agon/timer.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#include "gpio.h"

#include "video_chartable_1.h"
#include "tms9918_mem_map.h"

// VDP Operation           RWB  MODE 
// ----------------------------------
// Write to VRAM            0     0  
// Write to Reg/Addr Set    0     1  
// Read from VRAM           1     0 
// Read Status Reg          1     1 

// define PINS (see gpio.h)
// data pins on Port C - all 8-bits

#define TMS_DATA           PC_DAT
#define TMS_DATA_DIR       PC_DIR

#define TMS_DATA_READ      0b11111111
#define TMS_DATA_WRITE     0b00000000

// NAM1 TMS9918 v2 board has 5 singal lines
//   cpu1-INT  - connected directly to 6502 IRQ
//   cpu2-WE   - connected to 6502 RWB line
//   cpu3-En   - Chip Select (originally only when Phi2 is High)
//   cpu5-A0   - Address 0 = MODE
//   cpu4-A1   - Address 1 select Write/Read reg address - make same as RWB
//
// INT - In the orignal design this wass connected to 6502 IRQ
// Equivalent in Z80 is INT ?)
// The VDP can generate interrupts for the CPU. It is used for 
// signaling the end of a line or sprite fine control
// I won't connect/use this for now
//
// WE - On a 6502 this is the RWB pin - We will use a GPIO port to write the correct signal to this line
//
// EN - set in GPIO port pin
//
// A0/A1 - these were used as convenient way to map 4 periperal addresses
// to the four VDP operations above.
// Actually, A0 is the MODE line to the VDP and A1 is equivalent to RWB

// Control signals on Port D (pins 4,5,6,7) - Drop IRQ from orig design
#define TMS_SIG             PD_DAT
#define TMS_SIG_DIR         PD_DIR
#define TMS_SIG_INIT        0b00001111

// Mode - this selects between VRAM=0 and Registers=1
// Pin 7 (count from 0)
#define TMS_SIG_MODE_BIT    7 

// Chip enable for accessing card (read/write)
// Pin 6
#define TMS_SIG_EN_BIT      6 

// RWB - CPU Read/Write Signal. High = R, Low = W
// Pin 5 
#define TMS_SIG_RWB_BIT     5 

// A1 - Make same as RWB (board needs it set)
// Pin 4 
#define TMS_SIG_A1_BIT      4 

static uint8_t VDPModeTable[32] = { VDP_SETUP_MODE0,VDP_SETUP_MODE1,VDP_SETUP_MODE2,VDP_SETUP_MODE3 };

bool tms_init();
void tms_write_data(uint8_t data);
uint8_t tms_read_data();
void tms_write_ctrl_addr(uint8_t data);
uint8_t tms_read_status();

void tms_write_vdp_register(uint8_t reg, uint8_t data);
uint8_t tms_read_vdp_status();
void tms_set_vdp_address(uint16_t vaddr, uint8_t read);
void tms_vdp_write_data(uint16_t vaddr, uint16_t datalen, uint8_t *data);
void tms_vdp_read_data(uint16_t vaddr, uint16_t datalen, uint8_t* buffer);

void tms_vdp_set_mode(uint8_t mode);
void tms_vdp_load_character_set();
void tms_vdp_clear_screen_m0( uint8_t clr_char );
void tms_memory_test(uint16_t addr, unsigned int count, bool reverse);
void read_vram(uint16_t addr, unsigned int count);

#endif
