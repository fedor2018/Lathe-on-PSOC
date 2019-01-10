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

void quad_init();
void step();

CY_ISR_PROTO(ISR_DIV_Handler);
CY_ISR_PROTO(ISR_DIV_CC_Handler);
CY_ISR_PROTO(ISR_DIV_UN_Handler);
CY_ISR_PROTO(ISR_QUAD_Handler);
CY_ISR_PROTO(QUAD_TC_Handler);
CY_ISR_PROTO(QUAD_COMP_Handler);
CY_ISR_PROTO(ISR_STEP_Handler);
CY_ISR_PROTO(ISR_RPM_Handler);

/* [] END OF FILE */
