#ifndef LCD1602_H_INCLUDED
#define LCD1602_H_INCLUDED

#define	pin_E	       GPIO_Pin_12
#define	pin_RS	       GPIO_Pin_11
#define pin_DB4        GPIO_Pin_10
#define pin_DB5        GPIO_Pin_2
#define pin_DB6        GPIO_Pin_1
#define pin_DB7	       GPIO_Pin_0

/*
 *   объявление функций
 */
extern void LCD_init(void);
extern void LCD_cmd4bit(uc8 cmd);
extern void LCD_data4bit(uc8 data);
extern void LCD_clearscreen(void);
extern void LCD_gotoxy(int column, int line);
extern void LCD_print(char *string);


#endif /* LCD1602_H_INCLUDED */
