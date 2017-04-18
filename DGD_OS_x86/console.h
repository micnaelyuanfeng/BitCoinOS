#ifndef _INCLUDE_CONSOLE_H_
#define _INCLUDE_CONSOLE_H_

#include "types.h"

typedef enum real_color{
  black = 0,
  blue = 1,
  green = 2,
  cyan = 3,
  red = 4,
  magenta = 5,
  brown = 6,
  light_grey = 7,
  dark_grey = 8,
  light_blue = 9,
  light_green = 10,
  light_cyan = 11,
  light_red = 12,  
  light_magenta = 13,
  light_brown = 14,
  white = 15
}real_color_t;

void console_clear();

void console_putc_color(char c, real_color_t back, real_color_t front);

//write blank and write string
void console_write(char * cstr);

void console_write_color(char *cstr, real_color_t back, real_color_t front);

void console_write_hex(uint32_t n, real_color_t back, real_color_t front);

void console_write_dec(uint32_t n, real_color_t back, real_color_t front);

void move_cursor(void);

void scoll(void);

#endif
