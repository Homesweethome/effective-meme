/*
**
**                           Main.c
**
**
**********************************************************************/
/*
   Last committed:     $Revision: 00 $
   Last changed by:    $Author: $
   Last changed date:  $Date:  $
   ID:                 $Id:  $

**********************************************************************/
#include "stm32f10x_conf.h"
#include "lcd1602.h"
#include <delay.h>

int main(void)
{
	// инициализируем LCD
	LCD_init();
    LCD_clearscreen();

    while(1)
    {
        Delay_ms(10000);
        LCD_clearscreen();
        Delay_ms(10000);
        LCD_print("HELLO");
        LCD_gotoxy(3,1);
        LCD_print("WORLD!");
    }
}
