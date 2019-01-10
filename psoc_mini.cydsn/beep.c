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
#include "beep.h"

//#define BEEP_DEBUG

void read_buz(char *s){
#ifdef BEEP_DEBUG
    extern char buf[21];
    sprintf(buf, "%s: %d,%d,%d %d-%d\r\n", s, 
        (uint16)BUZZER_ReadPeriod(),
        (uint16)BUZZER_ReadCounter(),
        (uint16)BUZZER_ReadStatus(),
        Pin_Buzzer_Read(),
        Pin_Led_Read());
    UART_UartPutString(buf);
#endif
}

void beep_init(){
    BEEP_OFF;
    BUZZER_Init();
    read_buz("s");
    ISR_BUZ_StartEx(ISR_BUZ_Handler);
}

void beep(uint16 delay){
    if(BUZZER_ReadStatus()==BUZZER_STATUS_RUNNING){
    read_buz("r");
        return;
    }
//    BUZZER_Stop();
    BUZZER_WritePeriod(1);
    BUZZER_WriteCounter(BEEP_DELAY_ms(delay));//
    read_buz("b");
    BEEP_ON;
    BUZZER_Start();
}

CY_ISR(ISR_BUZ_Handler){
    BEEP_OFF;
    read_buz("i");
    BUZZER_ClearInterrupt(BUZZER_INTR_MASK_TC);
}

/* [] END OF FILE */
