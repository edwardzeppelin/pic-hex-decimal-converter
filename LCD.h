#ifndef LCD_H
#define LCD_H

#include "C:/Projects/cursach/Try1/config.h"
#include <htc.h>

void init_LCD(void);
void clear_LCD(void);
void cursor(int isOn);
void blink(int isOn);
void set_cursor(int line, int column);
void show_str(char* data);
void show_int(int data);
void print_data(char data);
void print_command(char data);

#endif