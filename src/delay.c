#include <stm32f10x_tim.h>
#include <stm32f10x_rcc.h>
#include <stdint.h>
#include <delay.h>

/*
���������� �������� ����� ������ TIM2.
������ �����������, ������ ������ n - ����������� (��� �����),
� ����� �� ��������� ������������� ������
*/

// ������������� ������� ��� �������������� ��������
void _init_us(void) {
	_init_delay(DELAY_TIM_FREQUENCY_US);
}

// ������������� � ������ ������� ��� �������������� ��������
void _init_ms(void) {
	_init_delay(DELAY_TIM_FREQUENCY_MS);
}

void _init_delay(uint32_t delayTime){
	// �������� TIM2
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	// ������� ������������ �������
	TIM_TimeBaseInitTypeDef TIM;
	TIM_TimeBaseStructInit(&TIM);
	TIM.TIM_Prescaler = (SystemCoreClock / delayTime) - 1;
	TIM.TIM_Period = UINT16_MAX;
	TIM.TIM_ClockDivision = 0;
	TIM.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2, &TIM);

	// �������� ������� ��� TIM2
	TIM_Cmd(TIM2,ENABLE);
}

// ��������� �������
void _stop_timer(void) {
	TIM_Cmd(TIM2, DISABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, DISABLE); // ��, �������� �������?
}

// ������ �������� �� mSecs �����������
void Delay_ms(uint32_t mSecs) {
	// ������������� � ������ �������
	_init_ms();

	// ������ ���� � ������� ������� �����
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT - start) <= mSecs);

	// ��������� �������
	_stop_timer();
}

// ������ �������� �� uSecs �����������
void Delay_us(uint32_t uSecs) {
	// ������������� � ������ �������
	_init_us();

	// ������ ���� � ������� ������� �����
	volatile uint32_t start = TIM2->CNT;
	while((TIM2->CNT - start) <= uSecs);

	//��������� �������
	_stop_timer();
}
