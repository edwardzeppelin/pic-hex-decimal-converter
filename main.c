#include "C:/Projects/cursach/Try1/LCD.h"
#include <stdlib.h>
#include <string.h>

#define BUTTON_CONVERT RB4
#define BUTTON_CLEAR RB5
#define BUTTON_NEXT RB6
#define BUTTON_INCREASE RB7

char hex_string[5] = "0000";
int current_position = 0;
const char hex_chars[] = "0123456789ABCDEF";

void init_pic(void);
void on_buttons(void);
void off_buttons(void);
void process_input(void);
void convert_hex_to_dec(void);
void clear_input(void);
void print_hex_string(void);
void print_decimal(int value);
void increase_digit(void);

void main(void)
{
    init_pic();
    init_LCD();
    
    strcpy(hex_string, "0000");
    
    clear_LCD();
    set_cursor(0, 0);
    show_str("Hex: ");
    print_hex_string();
    
    set_cursor(1, 0);
    show_str("Dec: ");
    
    while(1)
    {
        on_buttons();
        process_input();
    }
}

void init_pic(void) 
{ 
    TRISB = 0xF0;
}

void on_buttons(void) 
{ 
    PORTB = 0xF0; 
}

void off_buttons(void) 
{ 
    PORTB = 0; 
}

void process_input(void)
{
    static unsigned char last_state = 0xFF;
    unsigned char current_state = PORTB & 0xF0;
    
    if (current_state != last_state)
    {
        __delay_ms(50);
        
        current_state = PORTB & 0xF0;
        
        if (current_state != last_state)
        {

            if (!(current_state & 0x10))
            {
                convert_hex_to_dec();
            }
            else if (!(current_state & 0x20)) 
            {
                clear_input();
            }

            else if (!(current_state & 0x40)) 
            {
                current_position = (current_position + 1) % 4;
                set_cursor(0, 6 + current_position);
            }

            else if (!(current_state & 0x80))
            {
                increase_digit();
            }
        }
        
        last_state = current_state;
    }
}

void increase_digit(void) 
{
    char current_char = hex_string[current_position];
    int index = 0;
    int i;
    

    for (i = 0; i < 16; i++)
    {
        if (hex_chars[i] == current_char)
        {
            index = i;
            break;
        }
    }
    

    index = (index + 1) % 16;
    hex_string[current_position] = hex_chars[index];
    

    set_cursor(0, 6 + current_position);
    print_data(hex_string[current_position]);
    set_cursor(0, 6 + current_position); 
}

void print_hex_string(void)
{
    int i;
    for (i = 0; i < 4; i++)
    {
        print_data(hex_string[i]);
    }
}

void convert_hex_to_dec(void)
{
    int decimal_value = 0;
    int digit_value;
    int i;
    

    for (i = 0; i < 4; i++)
    {
        char c = hex_string[i];
        if (c >= '0' && c <= '9')
            digit_value = c - '0';
        else if (c >= 'A' && c <= 'F')
            digit_value = c - 'A' + 10;
        else
            digit_value = 0;
            
        decimal_value = decimal_value * 16 + digit_value;
    }
    

    print_decimal(decimal_value);
}

void print_decimal(int value)
{
    char buffer[6];
    int i = 0;
    int temp = value;
    

    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        i = 1;
    }
    else
    {

        buffer[5] = '\0';
        i = 4;
        
        while (temp > 0 && i >= 0)
        {
            buffer[i] = (temp % 10) + '0';
            temp /= 10;
            i--;
        }
        i++;
    }
    

    set_cursor(1, 5);
    show_str("     "); 
    

    set_cursor(1, 5);
    show_str(&buffer[i]);
}

void clear_input(void)
{
    strcpy(hex_string, "0000");
    current_position = 0;
    
    set_cursor(0, 6);
    print_hex_string();
    set_cursor(0, 6);
    

    set_cursor(1, 5);
    show_str("     ");
}