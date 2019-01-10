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


void key_init();
void joy_scan();
void MenuKeySelectPressed();
void MenuKeyUpPressed();
void MenuKeyDownPressed();
void lcd_row1();
void lcd_row2();
void lcd_row3();
void lcd_row4();
void LimitLeft();
void LimitRight();
const char* fmt_state();

CY_ISR_PROTO(ISR_KEY_Handler);
CY_ISR_PROTO(ISR_ADC_Handler);
CY_ISR_PROTO(ADC_IRQ_Handler);

enum Pressed_Key{
    Joy_Left=0,
	Joy_Right,
	Limit_Left_Button,
	Limit_Right_Button,
	Key_Select,
    Key_Down,
    Key_Up,
    Key_None=10
};

enum Mode{
  Mode_Tpi = 0,
  Mode_Feed,
	Mode_Thread,
	Mode_None
};

typedef struct{
	byte Mode:3;// Mode
	byte Feed;
	byte Thread;
	byte Tpi;
	uint16_t Ogr;
} MODE_INFO;

typedef struct{
	byte joy;
    bool fast;
    uint16 rpm;
} FLAG_INFO;

enum State{
    st_stop=0,
    st_wait,//wait zero
    st_accel,
    st_run,
    st_decel,
    st_fast,
    st_end //wait stop
};

typedef struct {
    uint8 state;
    int16 accel;
    int16 ks_tmp;//ks_div+accel;
    uint16 ks_acc;//accel
    int16 ks_div;
    int16 ks_fast;
    int16 km_cnt;
    int16 km_div;
    long lim_left;
    long lim_right;
} MOVE_INFO;

/* [] END OF FILE */
