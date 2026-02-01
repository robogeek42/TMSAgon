/*
  vim:ts=4
  vim:sw=4
*/
#include "util.h"

void clear_keys()
{
	struct keyboard_event_t e;
    while (kbuf_poll_event(&e)) {}
}
uint8_t wait_for_key(uint8_t key)
{
	struct keyboard_event_t e;
    do {
		while (!kbuf_poll_event(&e)) {}
	} while (e.vkey != key);
    return key;
}
uint8_t wait_for_any_key()
{
	struct keyboard_event_t e;
    while (!kbuf_poll_event(&e)) {}
    return e.vkey;
}
uint8_t wait_for_any_key_press()
{
	struct keyboard_event_t e;
    while (!kbuf_poll_event(&e)) {}
    return e.vkey;
}
void wait_clock( clock_t ticks )
{
	clock_t ticks_now = clock();

	do {
	} while ( clock() - ticks_now < ticks );
}


