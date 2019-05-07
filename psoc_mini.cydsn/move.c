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

//#define QUAD_DEBUG
//#define DIV_DEBUG

uint16 volatile rpm=0;
int32 volatile Motor_Z_Pos=0;
extern MODE_INFO mode;
extern FLAG_INFO flag;
extern MOVE_INFO move;

void inline div_print(char s, uint8 st){
#ifdef DIV_DEBUG
    if(st!=move.state){
        char b[128];
        sprintf(b,"D%c %4s z=%7ld t=%5d c=%5d f=%5d d=%5d a=%u l=%7ld|%7ld\r\n",s,
            fmt_state(move.state), Motor_Z_Pos,
            move.ks_tmp,move.km_cnt,
            move.ks_fast,move.ks_div, 
            move.ks_acc,
            move.lim_left,move.lim_right);
        UART_UartPutString(b);
    }
#endif
}

void inline quad_zero(char s){
#ifdef QUAD_DEBUG
    char b[32];
    sprintf(b,"Z%c%u %x cnt=%u p=%u cmp=%u rpm=%lu\r\n",s,
        Pin_CCW_Read(),
        QUAD_ReadStatusRegister(),  QUAD_ReadCounter(), 
        QUAD_ReadPeriod(),QUAD_ReadCompare(),RPM_ReadCounter());
    UART_UartPutString(b);
#else
    QUAD_ReadStatusRegister();
#endif
    rpm=RPM_ReadCounter();
    if(move.state==st_wait){
        div_print('w',st_accel);
        move.state=st_accel;
        //move.ks_tmp=move.ks_acc= move.ks_div+((RPM_V(rpm)/move.ks_div)*10);//?
        DIV_WriteCounter(move.ks_tmp);
        DIV_Start();
    }
    Pin_Zero_Write(~Pin_Zero_Read());
    RPM_WriteCounter(0);
    RPM_Start();
    flag.rpm=0;
}

void move_init(){
    move.state=st_stop;
    QUAD_Start();
    QUAD_WriteCompare(QUAD_MAX);
    QUAD_WriteCounter(QUAD_MAX/2);
    QUAD_WritePeriod(QUAD_MAX-1);
    ISR_QUAD_TC_StartEx(QUAD_TC_Handler);
    ISR_QUAD_COMP_StartEx(QUAD_COMP_Handler);

    DIV_Init();
    DIV_WriteCompare(1);
    ISR_DIV_StartEx(ISR_DIV_Handler);

    RPM_Init();
    RPM_WritePeriod(65534);
}

CY_ISR(QUAD_TC_Handler){//DN
    quad_zero('T');
    SPINDEL_CCW;
}

CY_ISR(QUAD_COMP_Handler){//UP
    quad_zero('C');
    QUAD_WriteCounter(1);
    SPINDEL_ACW;
}

CY_ISR(ISR_RPM_Handler){
    rpm=0;
}

CY_ISR(ISR_DIV_Handler){
    if(DIV_GetInterruptSource()&DIV_INTR_MASK_CC_MATCH){//div==1
        DIV_ClearInterrupt(DIV_INTR_MASK_CC_MATCH);
//        STEP_ON;
        return;
    }
   STEP_OFF;//div==0
   if(MOVE_RIGHT){
        Motor_Z_Pos++;
        if(RIGHT_IS_ON){
            if(Motor_Z_Pos>=move.lim_right){
                move.state=st_end;
            }else{
                if(Motor_Z_Pos>=move.acc_right/*(move.lim_right-move.ks_acc)*/){
                    div_print('+', st_decel);
                    move.state=st_decel;
                }
            }
        }
    }else {
        Motor_Z_Pos--;
        if(LEFT_IS_ON){
            if(Motor_Z_Pos<=move.lim_left){
                move.state=st_end;
            }else{
                if(Motor_Z_Pos<=move.acc_left/*(move.lim_left+move.ks_acc)*/){
                    div_print('-',st_decel);
                    move.state=st_decel;
                }
            }
        }
    }
   switch(move.state){
        case st_accel:
            move.ks_tmp-=move.ks_inc;
            if(move.ks_tmp<=move.ks_div){
                div_print('a',st_run);
                move.state=st_run;
                move.km_cnt=0;
                move.ks_tmp=move.ks_div;
            }
            DIV_WriteCounter(move.ks_tmp-1);
            break;
        case st_fast:
        case st_run:
            if(flag.fast){
                DIV_WriteCounter(move.ks_fast);
            }else{
                move.km_cnt+=move.km_div;
                if(move.km_cnt>move.km_div){
                    move.km_cnt-=10000;
                    DIV_WriteCounter(move.ks_tmp-1);
                }else{
                    DIV_WriteCounter(move.ks_tmp-2);
                }
            }
            break;
        case st_decel:
            if(move.ks_tmp < (/*move.ks_div+*/move.ks_acc)){///ks_div+accel
                move.ks_tmp+=move.ks_inc;
                DIV_WriteCounter(move.ks_tmp-1);
                break;
            }
        case st_end:
        case st_stop:
        default:
            div_print('e',st_stop);
            move.state=st_stop;
            move.ks_tmp=move.ks_acc;
            DIV_ClearInterrupt(DIV_INTR_MASK_TC);
            DIV_Stop();
            return;//
            break;
    }
    
    DIV_ClearInterrupt(DIV_INTR_MASK_TC);
    STEP_ON;//div==0
    DIV_Start();
}
/* [] END OF FILE */
