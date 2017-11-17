#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stdint.h>
#include <delay.h>

/*
Реализация задержки через таймер TIM2.
Таймер запускается, тикает каждые n - миллисекунд (или микро),
а когда всё отсчитано останавливаем таймер
*/

// Инициализация таймера для микросекундных задержек
void _init_us(void) {
	_init_delay(DELAY_TIM_FREQUENCY_US);
}

// Инициализация и запуск таймера для миллисекундных задержек
void _init_ms(void) {
	_init_delay(DELAY_TIM_FREQUENCY_MS);
}

void _init_delay(uint32_t delayTime){
	// включаем TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// базовая конфигурация времени
	TIM_TimeBaseInitTypeDef TIM;
	TIM_TimeBaseStructInit(&TIM);
	TIM.TIM_Prescaler = (SystemCoreClock / delayTime) - 1;
	TIM.TIM_Period = UINT16_MAX;
	TIM.TIM_ClockDivision = 0;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM);

	// Включаем счётчик для TIM2
	TIM_Cmd(TIM2,ENABLE);
}

// остановка таймера
void _stop_timer(void) {
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); // хз, экономия энергии?
}

// делаем задержку на mSecs миллисекунд
void Delay_ms(uint32_t mSecs) {
	// инициализация и запуск таймера
	_init_ms();

	// просто цикл в котором сверяем время
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT - start) <= mSecs);

	// остановка таймера
	_stop_timer();
}

// делаем задержку на uSecs микросекунд
void Delay_us(uint32_t uSecs) {
	// инициализация и запуск таймера
	_init_us();

	// просто цикл в котором сверяем время
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT - start) <= uSecs);

	//остановка таймера
	_stop_timer();
}
