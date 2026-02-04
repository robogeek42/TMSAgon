#ifndef TMS_LOWLEVEL_H
#define TMS_LOWLEVEL_H

#define PB_DAT 0x009A
#define PB_DIR 0x009B
#define PC_DAT 0x009E
#define PC_DIR 0x009F
#define PD_DAT 0x00A2
#define PD_DIR 0x00A3

extern uint8_t load_mode(uint8_t mode);
//extern void io_out(int addr, uint8_t value);

#endif
