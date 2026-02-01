/*
  vim:ts=4
  vim:sw=4
*/
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

    //tms_memory_test();

    //tms_vdp_load_character_set();
    
    //tms_vdp_clear_screen_m0(0x21);

    printf("Bye!\n");
	kbuf_deinit();
    return 0;
}

