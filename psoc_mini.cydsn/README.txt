
01234567890123456789|
  Thread: x.xxmm    |
  Thread: x.xxbpi   |
    Feed: x.xxmm    |
  CCW Ang xxx.x     |
  ACW RPM  xxxx     |
    << -zzz.zz >>   | 
-xxx.xx SSSS -yyy.yy|

SSSS:
    RUN
    WAIT
    END
    STOP
    ACCEL
    DECEL
    FAST

настройки:
#define Screw_mm                2.0 //шаг винта
#define Motor_Z_Step_Per_Rev    200      // Кол-во шагов/об, ось X
#define QUAD_RES                2000//энкодер дел/об
#define MicroStep_Z             4   // Микрошаг
#define ACCEL_N                 20 // accel* steps

variable:
#define QUAD_MAX                (QUAD_RES*4)
#define STEP_PER_MM        		((Motor_Z_Step_Per_Rev*MicroStep_Z)/Screw_mm)
#define TICK_PER_STEP_MM        (QUAD_MAX/STEP_PER_MM)
#define ACCEL                   (MicroStep_Z * ACCEL_N) // accel* steps
-----------------------
move.ks_div *_STEP_I
move.km_div *_STEP_D
move.ks_tmp move.ks_div+ACCEL
move.km_cnt 0
ACCEL // accel->steps
move.ks_fast *_MM_STEP_I - max
=====
accel move.ks_tmp->move.ks_div
run   move.km_cnt+move.km_div->move.ks_tmp(0|-1)
decel move.ks_tmp->(move.ks_div+ACCEL)
------------------------
accel rpm/move.ks_div



