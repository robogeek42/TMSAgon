#include "gpio.h"

inline uint8_t io_in(int addr)
{
    uint8_t data;
    data = *((volatile uint8_t __attribute__((address_space(3))) *)addr);
    vdp_set_text_colour(1); printf("READ  %04X    ->    %02X\n", addr, data);
    vdp_set_text_colour(7);
    return data;
}

inline void io_out(int addr, uint8_t value)
{
    vdp_set_text_colour(2);printf("WRITE %04X %02X\n", addr, value);
    *((volatile uint8_t __attribute__((address_space(3))) *)addr) = value;
    vdp_set_text_colour(7);
}
