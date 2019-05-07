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
#include "project.h"
#include <stdio.h>

#define VER "0.4b"
void version(char *sversion);
void div_print(char s, uint8 st);

#define byte uint8
typedef unsigned char bool;
#define false 0
#define true (!false)
#define NELEMS(x)  (sizeof(x) / sizeof((x)[0]))
#define space_str(s)    memset(s,' ',sizeof(s))

void move_init();
/**/
#define LEFT_ON         Pin_Left_Write(1)
#define LEFT_OFF        Pin_Left_Write(0)
#define LEFT_IS_ON      (Pin_Left_Read()!=0)
#define RIGHT_ON        Pin_Right_Write(1)
#define RIGHT_OFF       Pin_Right_Write(0)
#define RIGHT_IS_ON     (Pin_Right_Read()!=0)
#define STEP_ON         (Pin_Step_Write(0))
#define STEP_OFF        (Pin_Step_Write(1))
#define STEP_IS_ON      (Pin_Step_Read()==0)
#define SPINDEL_CCW     (Pin_CCW_Write(0))
#define SPINDEL_ACW     (Pin_CCW_Write(1))
#define DIR_LEFT        0
#define DIR_RIGHT       1
#define MOVE_LEFT       ((Pin_Dir_Read()&1)==DIR_LEFT)
#define MOVE_RIGHT      ((Pin_Dir_Read()&1)==DIR_RIGHT)
/**/
#define RPM_FREQ                30000 //rpm clock counter
#define MicroStep_Z             2   // Микрошаг
//#define Screw_len               500 //длина винта mm
#define Screw_mm                2.0 //шаг винта
#define QUAD_RES                2000//энкодер дел/об
#define QUAD_MAX                (QUAD_RES*4)
#define RPM_V(r)                (uint16)((RPM_FREQ*60)/r)//
#define STEP_DELAY  12
#define Motor_Z_Step_Per_Rev    200      // Кол-во шагов/об, ось X
#define STEP_PER_MM        		((Motor_Z_Step_Per_Rev*MicroStep_Z)/Screw_mm)
#define Motor_Z_MM(z)           ((z*100)/STEP_PER_MM)
//#define ACCEL                   50 // accel->steps
//#define TICK_PER_STEP_MM        (QUAD_MAX/((Motor_Z_Step_Per_Rev*MicroStep_Z)/Screw_mm))
#define TICK_PER_STEP_MM        (QUAD_MAX/STEP_PER_MM)
#define FEED_MM_STEP(mm) 		(uint16_t)(TICK_PER_STEP_MM*100/mm)
#define THREAD_MM_STEP_I(mm) 	(uint16_t)(TICK_PER_STEP_MM*100/mm)
#define THREAD_MM_STEP_D(mm) 	(uint16_t)(((float)(TICK_PER_STEP_MM*100/mm)-THREAD_MM_STEP_I(mm))*10000.0)
#define THREAD_TPI_STEP_I(tpi) 	(uint16_t)(TICK_PER_STEP_MM/(25.4/tpi))
#define THREAD_TPI_STEP_D(tpi) 	(uint16_t)(((float)(TICK_PER_STEP_MM/(25.4/tpi))-THREAD_TPI_STEP_I(tpi))*10000.0)
#define ENC_ANG(pos) ((uint32)(pos*3600)/QUAD_MAX)

/* [] END OF FILE */
