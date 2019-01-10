/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include "main.h"

#define BEEP_DELAY_ms(m)  (1*m)//1kHz->1ms
#define BEEP_ON     Pin_Buzzer_Write(0);Pin_Led_Write(1);
#define BEEP_OFF    Pin_Buzzer_Write(1);Pin_Led_Write(0);

void beep_init();
void beep(uint16 delay);
void read_buz(char *s);
CY_ISR_PROTO(ISR_BUZ_Handler);

/* [] END OF FILE */
