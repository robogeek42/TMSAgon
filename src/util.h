/*
  vim:ts=4
  vim:sw=4
*/
#ifndef _UTIL_H
#define _UTIL_H

#include "agon/vdp.h"
#include "agon/mos.h"
#include "agon/keyboard.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define COL(C) vdp_set_text_colour(C)
#define TAB(X,Y) vdp_cursor_tab(X,Y)


void wait_clock( clock_t ticks );

// clear keyboard buffer
void clear_keys();
// return after specific key is pressed
uint8_t wait_for_key(uint8_t key);
// return after key is released
uint8_t wait_for_any_key();
// return after key-down event
uint8_t wait_for_any_key_press();

#endif
