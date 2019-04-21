#ifndef _COG22_H
#define _COG22_H

#include "sys.h"
#include "font.h"


#ifndef uchar
#define uchar unsigned char
#endif

#ifndef uint
#define uint unsigned int
#endif


#define rs PAout(3) 
#define reset PAout(4) 
#define cs1 PAout(5) 
#define lcd_sclk PAout(2) 
#define lcd_sid PAout(1) 


void transfer_command(int data1);
void transfer_data(int data1);
void delay(int i);
void initial_lcd(void);
void lcd_address(uchar page,uchar column);
void clear_screen(void);
void test_display(uchar data1,uchar data2);
void display_string_8x16(uchar fb,uint page,uint column,uchar *text);
void display_string_8x16_t(uchar fb,uint page,uint column,uchar text);
void display_string_5x8(uint page,uint column,uchar *text);
void display_graphic_16x16(uchar fb,uchar page,uchar column,uchar *dp);
void display_bk(void);





















#endif

