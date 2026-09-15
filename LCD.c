#include "C:/Projects/cursach/Try1/LCD.h"
#include <string.h>
#define EN RB2
#define RS RB1
int cursor_pos = 0;

void print_command(char command);
void print_data(char data);
void Delay(unsigned int tmp);
void Pulse(unsigned int x);
void print_side(int side);
void on_buttons_LCD();
void off_buttons_LCD();
int check_buttons_LCD();

void set_cursor(int line, int column){
   cursor_pos = column;
   if (line == 0) {
      print_command(0x80+column);
   }
   else {
      print_command(0xC0+column);
   }
}

void print_side(int side){
   char data[13];
   clear_LCD();
   if (side==1){
      strcpy(data,"left button\0");
   }
   else{
      strcpy(data,"right button\0");
   }
   int i=0;
   while (data[i]!='\0'){
      print_data(data[i++]);
   }
}

int mpow(int data, int y) {
   int tmp = 1;
   for(int i = 0; i < y ; i++) tmp *= data;
   return tmp;
}

void show_int(int data) {
   int cursor_buf = cursor_pos;
   if (data == 0) { print_data(0x30); return;}
   int size = 0, td = data;
   while (td > 0) { td/= 10; size++; }
   while (td < size) {
      if (td == 0)
      print_data(0x30 |
      (data / mpow(10, size - 1)));
      else
      print_data(0x30 | (data / mpow(10, size - 1 - td) % 10));
      td++;
      cursor_pos++;
   }
}

void Delay(unsigned int tmp){
   while(tmp--);
   return;
}

void Pulse(unsigned int x){
   EN=1;
   Delay(x);
   EN=0;
   Delay(x);
}

void init_LCD()
{
   Delay(1000);
   TRISB = 0;
   PORTB = 0x30;
   Pulse(100);
   Pulse(100);
   Pulse(100);
   PORTB=0x20;
   Pulse(100);
   print_command(0x28);
   print_command(0x0C);
   print_command(0x06);
   print_command(0x02);
}

void print_data(char data){
   on_buttons_LCD();
   while(!check_buttons_LCD());
      RS=1;
      PORTB=(PORTB&0x0F)+(data&0xF0);
      Pulse(10);
      PORTB = (PORTB&0x0F)+(data<<4);
      Pulse(10);
      off_buttons_LCD();
}

void print_command(char data){
   on_buttons_LCD();
   while(!check_buttons_LCD());
   RS=0;
   PORTB=(PORTB&0x0F)+(data&0xF0);
   Pulse(100);
   PORTB = (PORTB&0x0F)+(data<<4);
   Pulse(100);
   RS=1;
   off_buttons_LCD();
   Delay(1000);
}

void show_str(char* data)
{
    int i = 0;
    while (data[i] != '\0')
    {
        print_data(data[i]);
        i++;
    }
}

void clear_LCD() { print_command(0x01); }
void on_buttons_LCD() { PORTB = 0xF0; }
void off_buttons_LCD() { PORTB = 0; }
int check_buttons_LCD(){ return (PORTB & 0xF0) == 0xF0; }