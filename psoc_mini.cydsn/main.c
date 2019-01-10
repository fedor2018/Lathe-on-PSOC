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
#include "beep.h"
#include "move.h"
#include "key.h"

char buf[80];//21
#define START_MSG0 "  Lathe on PSOC "
extern FLAG_INFO flag;
extern uint16 volatile rpm;

void clear_row(uint8 r){
    char b[21];
    space_str(b);
    LCD_Position(r, 0u);//row, col
    LCD_PrintString(b);    
}

void init(){
    UART_Start();
    beep_init();
    LCD_Start();
    LCD_ClearDisplay();
    move_init();
    key_init();
    CyGlobalIntEnable; /* Enable global interrupts. */
    LCD_Position(0u, 0u);//row, col
    LCD_PrintString(START_MSG0);
    LCD_Position(2u, 0u);//row, col
    version(buf);
    LCD_PrintString(buf);
    LEFT_ON;
    RIGHT_ON;
    beep(100);
    CyDelay(500);
    LEFT_OFF;
    RIGHT_OFF;
    UART_UartPutString(START_MSG0);
    UART_UartPutCRLF(' ');
    UART_UartPutString(buf);
    UART_UartPutCRLF(' ');
    CyDelay(500);
}

int main(void)
{
    init();
    LCD_ClearDisplay();
    lcd_row1();
    clear_row(2);
    for(;;){
        if(Pin_SW_Read()==0){
            CyDelay(500);
            if(Pin_SW_Read())CySoftwareReset();
        }
        joy_scan();
        lcd_row2();
        lcd_row3();
        lcd_row4();
        Pin_Led_Write(~Pin_Led_Read());
        CyDelay(50);
        if(flag.rpm++>20){//1s/50ms
            rpm=0;
            flag.rpm=0;
        }
    }
}

/* [] END OF FILE */
