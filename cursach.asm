#include "LCD.h"
#include <stdlib.h>
#include <string.h>

#define BUTTON_CONVERT RB4
#define BUTTON_CLEAR RB5
#define BUTTON_NEXT RB6

char hex_string[5] = "0000";
int current_position = 0;
char hex_chars[] = "0123456789ABCDEF";

void init_pic();
void on_buttons();
void off_buttons();
void process_input();
void convert_hex_to_dec();
void clear_input();
void print_hex_string();
void print_decimal(int value);

void main(void)
{
    init_pic();
    init_LCD();
    
    // Инициализация строки нулями
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

void init_pic() 
{ 
    TRISB = 0xF0;  // RB4-RB7 как входы (кнопки), RB0-RB3 как выходы
}

void on_buttons() 
{ 
    PORTB = 0xF0; 
}

void off_buttons() 
{ 
    PORTB = 0; 
}

void process_input()
{
    static unsigned char last_state = 0xFF;
    unsigned char current_state = PORTB & 0xF0;
    
    // Проверка изменения состояния кнопок
    if (current_state != last_state)
    {
        __delay_ms(50);  // Дребезг
        
        current_state = PORTB & 0xF0;
        
        if (current_state != last_state)
        {
            // Кнопка преобразования
            if (!(current_state & 0x10))  // RB4
            {
                convert_hex_to_dec();
            }
            // Кнопка очистки
            else if (!(current_state & 0x20))  // RB5
            {
                clear_input();
            }
            // Кнопка следующего символа
            else if (!(current_state & 0x40))  // RB6
            {
                current_position = (current_position + 1) % 4;
                set_cursor(0, 6 + current_position);
            }
            // Кнопка увеличения значения (RB7)
            else if (!(current_state & 0x80))
            {
                increase_digit();
            }
        }
        
        last_state = current_state;
    }
}

void increase_digit()
{
    char current_char = hex_string[current_position];
    int index = 0;
    
    // Находим текущий символ в массиве hex_chars
    for (int i = 0; i < 16; i++)
    {
        if (hex_chars[i] == current_char)
        {
            index = i;
            break;
        }
    }
    
    // Увеличиваем на 1 (циклически)
    index = (index + 1) % 16;
    hex_string[current_position] = hex_chars[index];
    
    // Обновляем отображение
    set_cursor(0, 6 + current_position);
    print_data(hex_string[current_position]);
    set_cursor(0, 6 + current_position);  // Возвращаем курсор
}

void print_hex_string()
{
    for (int i = 0; i < 4; i++)
    {
        print_data(hex_string[i]);
    }
}

void convert_hex_to_dec()
{
    int decimal_value = 0;
    int digit_value;
    
    // Преобразуем hex строку в число
    for (int i = 0; i < 4; i++)
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
    
    // Выводим десятичное значение
    print_decimal(decimal_value);
}

void print_decimal(int value)
{
    char buffer[6];
    int i = 0;
    
    // Преобразуем число в строку
    if (value == 0)
    {
        buffer[0] = '0';
        buffer[1] = '\0';
        i = 1;
    }
    else
    {
        // Заполняем буфер с конца
        buffer[5] = '\0';
        i = 4;
        
        while (value > 0 && i >= 0)
        {
            buffer[i] = (value % 10) + '0';
            value /= 10;
            i--;
        }
        i++;  // Корректируем индекс
    }
    
    // Очищаем область вывода
    set_cursor(1, 5);
    show_str("     ");  // 5 пробелов для очистки
    
    // Выводим число
    set_cursor(1, 5);
    show_str(&buffer[i]);
}

void clear_input()
{
    strcpy(hex_string, "0000");
    current_position = 0;
    
    set_cursor(0, 6);
    print_hex_string();
    set_cursor(0, 6);
    
    // Очищаем нижнюю строку
    set_cursor(1, 5);
    show_str("     ");
}