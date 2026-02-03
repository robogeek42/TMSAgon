/*
  vim:ts=4
  vim:sw=4
*/

#define DEBUG 0

#include <stdio.h>
#include <stdlib.h>

#include "agon/vdp.h"
#include "agon/mos.h"

#include "util.h"
#include "tms.h"

int main(/*int argc, char *argv[]*/)
{
	vdp_vdu_init();

	kbuf_init(16);

    printf("Initialise TMS board\n");
    // Initialise the TMS board
    tms_init();

    //tms_memory_test(0x800,8, false);
    //tms_memory_test(0x800,8, true);

    printf("Load character set\n");
    tms_vdp_load_character_set();
    //read_vram(0x00,256);
    
    tms_memory_test(0x800,256, false);
    printf("Clear screen\n");
    tms_vdp_clear_screen_m0(0x2B);
    //tms_memory_test(0x880,128, true);

    read_vram(0x800,256);
    
    printf("Bye!\n");
	kbuf_deinit();
    return 0;
}

