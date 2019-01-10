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
#include "key.h"
#include "beep.h"
#include <stdlib.h>

uint16 joy=0;
extern int32 volatile Motor_Z_Pos;
extern uint16 volatile rpm;

uint8 Feeds[]={3,6,9,12,15,18,21};//mm*100
#define Feed_idx NELEMS(Feeds)
uint16 Thread[]={25,30,35,40,50,60,70,75,80,100,125,150,175,200};//,250,300,400,500,600};//mm*100
#define Thread_idx NELEMS(Thread)
//int Thread_rpm[]={1200,100};
uint8 Thread_tpi[]={80,72,64,60,56,48,44,40,36,32,28,27,26,24,22,20,19,18,16,14,12};//,10,9,8,7,6,5,4};//inch*1
#define Tpi_idx NELEMS(Thread_tpi)
//int Tpi_rpm[]={1200,100};

MODE_INFO mode;
FLAG_INFO flag;
MOVE_INFO move;

void key_init(){
    mode.Mode=Mode_Feed;
    mode.Feed=2;
    mode.Thread=9;
    mode.Tpi=0;
    flag.joy=Key_None;
    flag.rpm=0;
    lcd_row1();

    ADC_Start();
    ISR_ADC_StartEx(ISR_ADC_Handler);
    ADC_StartConvert();
    ISR_KEY_ClearPending();
    ISR_KEY_StartEx(ISR_KEY_Handler); 
}

CY_ISR(ISR_KEY_Handler){
    flag.fast=Pin_Fast_Read()?0:1;
    div_print('f',st_stop);
}

CY_ISR(ISR_ADC_Handler){/* 0-156-261-350-450-550-650-750-850-950-1024 */
    static int16 old=0;
    int16 j=ADC_GetResult16(0);
    if(j==old){
        joy=j/200;//12bit->0-10
    }
    old=j;
}

void lcd_row1(){
    char buf[21];
    space_str(buf);
    LCD_Position(0u, 0u);//row, col
    LCD_PrintString(buf);
    LCD_Position(0u, 2u);//row, col
    switch(mode.Mode){
        case Mode_Thread:
            snprintf(buf,15,"Thread: %d.%2.2dmm   ", Thread[mode.Thread]/100, Thread[mode.Thread]%100);
            move.ks_div=THREAD_MM_STEP_I(Thread[mode.Thread]);
            move.km_div=THREAD_MM_STEP_D(Thread[mode.Thread]);
            move.ks_fast=THREAD_MM_STEP_I(Thread[Thread_idx-1]);
            break;
        case Mode_Tpi:
            snprintf(buf,15,"Thread:  %2dtpi   ", Thread_tpi[mode.Tpi]);
            move.ks_div=THREAD_TPI_STEP_I(Thread_tpi[mode.Tpi]);
            move.km_div=THREAD_TPI_STEP_D(Thread_tpi[mode.Tpi]);
            move.ks_fast=THREAD_TPI_STEP_I(Thread_tpi[Tpi_idx-1]);
            break;
        case Mode_Feed:
        default:
            snprintf(buf,15,"  Feed: 0.%2.2dmm   ", Feeds[mode.Feed]);
            move.ks_div=FEED_MM_STEP(Feeds[mode.Feed]);
            move.km_div=0;
            move.ks_fast=FEED_MM_STEP(Feeds[Feed_idx-1]);
            break;
    }
    move.ks_acc=move.ks_div+ACCEL;
    move.ks_tmp=move.ks_div+move.ks_acc;
    move.km_cnt=0;
    LCD_PrintString(buf);
/*    UART_UartPutString(buf);
    UART_UartPutCRLF(' ');
    sprintf(buf,"div=%u.%u\r\n", move.ks_div,move.km_div);
    UART_UartPutString(buf);   
    sprintf(buf,"tmp=%u f=%u\r\n", move.ks_tmp,move.ks_fast);
    UART_UartPutString(buf);   */
}

void lcd_row2(){
    char b[21];
    space_str(b);
    LCD_Position(1u, 0u);//row, col
    sprintf(b+2,"%s     ",Pin_CCW_Read()?"CCW":"ACW");
    if(rpm>61000 || rpm==0){
        uint32 ang=ENC_ANG(QUAD_ReadCounter());
        snprintf(b+7,12, "Ang %3u.%1.1u  ", (uint16)(ang/10), (uint16)(ang%10));
    }else{
        snprintf(b+7,12, "RPM  %5u  ", RPM_V(rpm));
    }
    LCD_PrintString(b);
}

void fmt_pos(char *b){
    long pos=(Motor_Z_Pos/STEP_PER_MM)*100;//Motor_Z_MM(Motor_Z_Pos);
    snprintf(b,8,"%c%u.%02u  ",
        (Motor_Z_Pos>=0)?' ':'-',
        abs((int16)(pos/100)), 
        abs(pos%100));
}


const char* fmt_state(){
    if(flag.fast){
        return "FAST";
    }
    switch(move.state){
        case st_accel:
            return "ACCL";
            break;
        case st_decel:
            return "DECL";
            break;
        case st_run:
            return "RUN";
            break;
        case st_stop:
            return "STOP";
            break;
        case st_wait:
            return "WAIT";
            break;
        case st_fast:
            return "FAST";
            break;
        case st_end:
            return "END";
            break;
        default:
            return "???";
            break;
    }
}

void lcd_row3(){
    char buf[21];
    long pos=(Motor_Z_Pos/STEP_PER_MM)*100;//Motor_Z_MM(Motor_Z_Pos);

    LCD_Position(2u, 0u);//row, col
    snprintf(buf,20,"%5s  %c%u.%02u %5s",
        ((joy==Joy_Left)?"<<":" "),
        ((Motor_Z_Pos>=0)?' ':'-'),
        abs((int16)(pos/100)), 
        abs(pos%100),
        ((joy==Joy_Right)?">>":" ")
    );
    LCD_PrintString(buf);
}

void lcd_row4(){
    char b[21];
    LCD_Position(3u, 7u);//row, col
    snprintf(b,6, " %4s ", fmt_state());
    LCD_PrintString(b);
}

void joy_scan(){
    if(joy!=flag.joy){
        if(joy<Key_None&&flag.joy<Key_None){//unknown
            flag.joy=joy;
            return;
        }else{
            if(flag.joy==Key_None){//none->move
                flag.fast=0;
                switch(joy){
                    case Joy_Left:
                        Pin_Dir_Write(DIR_LEFT);
                        if(move.state==st_stop)//
                            move.state=st_wait;
                        break;
                    case Joy_Right:
                        Pin_Dir_Write(DIR_RIGHT);
                        if(move.state==st_stop)//
                            move.state=st_wait;
                        break;
                    case Key_Down:
                        MenuKeyDownPressed();
                        break;
                    case Key_Up:
                        MenuKeyUpPressed();
                        break;
                    case Key_Select:
                        MenuKeySelectPressed();
                        break;
                    case Limit_Left_Button:
                        LimitLeft();
                        break;
                    case Limit_Right_Button:
                        LimitRight();
                        break;
                }
            }else{//move->end
                 switch(joy){
                    case Key_None:
                        switch(move.state){
                            case st_accel:
                            case st_fast:
                            case st_run:
                                div_print('n',st_decel);
                                move.state=st_decel;
                                break;
                            case st_end:
                            case st_wait:
                                DIV_ClearInterrupt(DIV_INTR_MASK_TC|DIV_INTR_MASK_CC_MATCH);
                                DIV_Stop();
                                move.state=st_stop;
                                STEP_OFF;
                                break;
                            case st_decel:
                            default:
                                break;
                        }
                        break;
                }
           }
        }
        flag.joy=joy;
        lcd_row1();
        beep(20);
    }
}

void MenuKeySelectPressed(){
    mode.Mode++;
    if(mode.Mode>=Mode_None){
        mode.Mode=0;
	}
}

void MenuKeyUpPressed(){
  switch (mode.Mode){
    case Mode_Thread:
      if (mode.Thread < Thread_idx-1){
        mode.Thread++;
      }
      break;
    case Mode_Tpi:
      if (mode.Tpi < Tpi_idx-1){
        mode.Tpi++;
      }
      break;
    case Mode_Feed:
      if (mode.Feed < Feed_idx-1){
        mode.Feed++;
      }
      break;
  }
}

void MenuKeyDownPressed(){
  switch (mode.Mode){
    case Mode_Thread:
      if (mode.Thread > 0){
        mode.Thread--;
      }
      break;
    case Mode_Tpi:
      if (mode.Tpi > 0){
        mode.Tpi--;
      }
      break;
    case Mode_Feed:
      if (mode.Feed > 0){
        mode.Feed--;
      }
      break;
  }
//	setParam();
}

void LimitSwap(){
    return;
    if(move.lim_left>move.lim_right){
        long tmp=move.lim_left;
        move.lim_left=move.lim_right;
        move.lim_left=tmp;
    }
}

void LimitLeft(){
    char b[16];
    
	if(move.state==st_stop){
        if (LEFT_IS_ON){//led on?
            LEFT_OFF;
            snprintf(b, 8,"%8s"," ");
        }else{
            if(RIGHT_IS_ON){//led on
                move.lim_left=Motor_Z_Pos;
            }else{
                move.lim_left=Motor_Z_Pos=0;
            }
            LEFT_ON;
            LimitSwap();
            fmt_pos(b);
        }
       	beep(10);
        LCD_Position(3u, 0u);//row, col
        LCD_PrintString(b);
	}
}

void LimitRight(){
    char b[16];
    
	if(move.state==st_stop){
        if (RIGHT_IS_ON){//led on?
            RIGHT_OFF;
            snprintf(b, 7,"%7s", " ");
        }else{
            if(LEFT_IS_ON){//led on
                move.lim_right=Motor_Z_Pos;
            }else{
                move.lim_right=Motor_Z_Pos=0;
            }
            RIGHT_ON;
            LimitSwap();
            fmt_pos(b);
        }
       	beep(10);
        LCD_Position(3u, 13u);//row, col
        LCD_PrintString(b);
	}
}

/* [] END OF FILE */
