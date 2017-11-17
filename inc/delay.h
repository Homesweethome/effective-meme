#ifndef DELAY_H_INCLUDED
#define DELAY_H_INCLUDED

#define DELAY_TIM_FREQUENCY_US 1000000		/* = 1MHZ -> ������ ������ ������������ */
#define DELAY_TIM_FREQUENCY_MS 1000			/* = 1kHZ -> ������ ������ ������������ */


/*
 *   ���������� �������
 */
extern void Delay_ms(uint32_t nTime);
extern void Delay_us(uint32_t nTime);


#endif /* DELAY_H_INCLUDED */
