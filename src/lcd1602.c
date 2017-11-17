#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <delay.h>
#include <lcd1602.h>

GPIO_InitTypeDef GPIO_LCD_Port_Struct;

// ������������� ������� ��� ������ � 4-������ �������
void LCD_init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//������������� ���� ��� LCD
	GPIO_LCD_Port_Struct.GPIO_Pin = pin_E | pin_RS | pin_DB4 | pin_DB5 | pin_DB6 | pin_DB7;
	GPIO_LCD_Port_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LCD_Port_Struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_LCD_Port_Struct);

	Delay_ms(30);              // ��� >=30����������� ����� ���� ��� ������ �������
	LCD_send4bit(0b00000011); // ������ ������������ 4-������ ����� (� �� ������� 8-������)
	Delay_ms(5);               // ��� ��� >4.1������������
	LCD_send4bit(0b00000011); // ����� ������ 4-������ ���� (�� ��� 8-������)
	Delay_us(150);             // ��� ��� >100�����������
	LCD_send4bit(0b00000011); // ����� ����������� 4-������ ���� (��� 8-������)
	LCD_send4bit(0b00000010); // ����� �������: 4-������ ���� (�������!)

	LCD_cmd4bit(0x28); // ������� ������: 2 ������, ������ �������� 5*7
	LCD_cmd4bit(0x0C); // ���������� �������: �����: ���, ������: ����
	LCD_cmd4bit(0x06); // ����� �����: ���������, �������� ���������
}

// ������� ������ �� E
void LCD_togglee(void) {
    GPIOB->BSRR = pin_E;
	Delay_us(4); // �� �������� ����� ����� �������� ~500�����������

    GPIOB->BRR = pin_E;
	Delay_us(4); // �� �������� ����� ����� �������� ~500�����������
}

// ���������� ����� ������� �� LCD ����� 4������ ����
// ���������� ������ �� ����� � ������ ������, ����� ������� ��������� ������
void LCD_send4bit(uc8 cmd) {
    GPIOB->BRR = pin_DB4;
    GPIOB->BRR = pin_DB5;
    GPIOB->BRR = pin_DB6;
    GPIOB->BRR = pin_DB7;

    if (cmd & (1<<0)) GPIOB->BSRR = pin_DB4;
    if (cmd & (1<<1)) GPIOB->BSRR = pin_DB5;
    if (cmd & (1<<2)) GPIOB->BSRR = pin_DB6;
    if (cmd & (1<<3)) GPIOB->BSRR = pin_DB7;

	LCD_togglee();
}

// �������� ������� �� LCD ����� 4 ������ ����
void LCD_cmd4bit(uc8 cmd) {
    GPIOB->BRR = pin_RS;
    LCD_send4bit(cmd>>4); // �������� ������� �������
    LCD_send4bit(cmd); // �������� ������ �������
    Delay_us(40); // � �������� ������� ����� ������������ ������� 39�����������
}

// �������� ������ �� LCD ����� 4 ������ ����
void LCD_data4bit(uc8 data) {
    GPIOB->BSRR = pin_RS;
    LCD_send4bit(data>>4);                 // �������� ������� �������
    LCD_send4bit(data);                    // �������� ������ �������
    GPIOB->BRR = pin_RS;
    Delay_us(44);                           // ������ � ������ ������� �������� 43������������
}

// �������� ������� ������� �� ������: ������� � ������
void LCD_gotoxy(int column, int line) {
    LCD_cmd4bit((column+(line<<6)) | 0x80);  // ��������� DDRAM ����� � ������������
}

// ������� ������ � ��������� ������� � ������
void LCD_clearscreen(void) {
	LCD_cmd4bit(0x01); // ������� ������
	Delay_ms(2); // ���������� �����. ��� 1.53������������
	LCD_cmd4bit(0x02); // ���������� ������ � ������
	Delay_ms(2); // ���������� �����. ��� 1.53������������
}

// ���������� ������ �� ������
void LCD_print(char *string) {
    while (*string) { LCD_data4bit(*string++); }
}
