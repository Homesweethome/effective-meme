#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <delay.h>
#include <lcd1602.h>

GPIO_InitTypeDef GPIO_LCD_Port_Struct;

// инициализация дисплея для работы с 4-битных режимом
void LCD_init(void) {

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	//устанавливаем пины для LCD
	GPIO_LCD_Port_Struct.GPIO_Pin = pin_E | pin_RS | pin_DB4 | pin_DB5 | pin_DB6 | pin_DB7;
	GPIO_LCD_Port_Struct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_LCD_Port_Struct.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_LCD_Port_Struct);

	Delay_ms(30);              // ждём >=30миллисекунд после того как подали питание
	LCD_send4bit(0b00000011); // просим использовать 4-битный режим (а он остаётся 8-битным)
	Delay_ms(5);               // ждём ещё >4.1миллисекунды
	LCD_send4bit(0b00000011); // опять просим 4-битную шину (всё ещё 8-битная)
	Delay_us(150);             // ждём ещё >100микросекунд
	LCD_send4bit(0b00000011); // вновь запрашиваем 4-битную шину (ещё 8-битная)
	LCD_send4bit(0b00000010); // набор функций: 4-битная шина (попалсо!)

	LCD_cmd4bit(0x28); // функции экрана: 2 строки, символ размером 5*7
	LCD_cmd4bit(0x0C); // Управление экраном: Экран: вкл, курсор: выкл
	LCD_cmd4bit(0x06); // Режим входа: инкремент, смещение отключено
}

// Дергаем ножкой по E
void LCD_togglee(void) {
    GPIOB->BSRR = pin_E;
	Delay_us(4); // по даташиту ждать нужно примерно ~500микросекунд

    GPIOB->BRR = pin_E;
	Delay_us(4); // по даташиту ждать нужно примерно ~500микросекунд
}

// Отправляем часть команды на LCD через 4битную шину
// Выставляем данные на пинах и дергаём ножкой, прося дисплей прочитать данные
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

// Отправка команды на LCD через 4 битную шину
void LCD_cmd4bit(uc8 cmd) {
    GPIOB->BRR = pin_RS;
    LCD_send4bit(cmd>>4); // отправке верхней тетрады
    LCD_send4bit(cmd); // отправка нижней тетрады
    Delay_us(40); // в основном команду нужно обрабатывать дисплею 39микросекунд
}

// отправка данных на LCD через 4 битную шину
void LCD_data4bit(uc8 data) {
    GPIOB->BSRR = pin_RS;
    LCD_send4bit(data>>4);                 // отправка верхней тетрады
    LCD_send4bit(data);                    // отправка нижней тетрады
    GPIOB->BRR = pin_RS;
    Delay_us(44);                           // данные в память пишутся примерно 43микросекунды
}

// указание позиции курсора на экране: столбец и строка
void LCD_gotoxy(int column, int line) {
    LCD_cmd4bit((column+(line<<6)) | 0x80);  // Указываем DDRAM адрес с координатами
}

// очистка экрана и установка курсора в начало
void LCD_clearscreen(void) {
	LCD_cmd4bit(0x01); // очистка экрана
	Delay_ms(2); // дисплейчик занят. ждём 1.53миллисекунды
	LCD_cmd4bit(0x02); // возвращаем курсор в начало
	Delay_ms(2); // дисплейчик занят. ждём 1.53миллисекунды
}

// отобразить строку на экране
void LCD_print(char *string) {
    while (*string) { LCD_data4bit(*string++); }
}
