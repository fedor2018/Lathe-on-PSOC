# Lathe-on-PSOC
Электронная гитара на PSOC CY8CKIT-049-42xx

Идея и алгоритмы:
- "Управление токарным на Atmega" https://www.chipmaker.ru/topic/97701/
- "Еще одна электронная гитара для токарника, на Ардуино" https://www.chipmaker.ru/topic/97701/

Для этого проекта ставились задачи:
- только набор минимальных функций
- простота реализации

Основные компоненты:
- шаговый двигатель NEMA 34
- энкодер E6B2-CWZ5B OMRON 2000P/R
- драйвер Leadshine DMA860H Driver DC 24-80V
- CY8CKIT-049-42xx
- LCD 20x4
- PSoC® Creator™ Integrated Design Environment (IDE)

Настройки в main.h:
#define Screw_mm                2.0 //шаг винта
#define Motor_Z_Step_Per_Rev    200 // Кол-во шагов/об, ось X
#define QUAD_RES                2000//энкодер дел/об
#define MicroStep_Z             4   // Микрошаг
#define ACCEL_N                 20  // accel* steps

