#include "gpio.h"

#define DEBUG 0

inline uint8_t io_in(int addr)
{
    uint8_t data;
    //data = *((volatile uint8_t __attribute__((address_space(3))) *)addr);
    data = IO(addr);
    if (DEBUG>1) {
        //vdp_set_text_colour(1);
        printf("READ  %04X    ->    %02X\n", addr, data);
    }
    if (DEBUG>1) {
        //vdp_set_text_colour(7);
    }
    return data;
}

inline void io_out(int addr, uint8_t value)
{
    if (DEBUG>1) {
        //vdp_set_text_colour(2);
        printf("WRITE %04X %02X\n", addr, value);
    }
    //*((volatile uint8_t __attribute__((address_space(3))) *)addr) = value;
    IO(addr) = value;
    if (DEBUG>1) {
        //vdp_set_text_colour(7);
    }
}
