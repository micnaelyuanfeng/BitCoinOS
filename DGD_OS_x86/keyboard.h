#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "types.h"

uint8_t keymap[200];

void keymap_setup(uint8_t * keymap);
void init_keyboard(void);

#endif
