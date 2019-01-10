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
#include <stdio.h>
#include <string.h>
#include "main.h"

void version(char *sversion)
{
  const char data[]=__DATE__;
  const char tempo[]=__TIME__;
  const char nomes[] = "JanFebMarAprMayJunJulAugSepOctNovDec";
  char omes[4];
  int ano, mes, dia, hora, min, seg;
//  char sversion[]="130910.001339";
//  double fv;

  if(strlen(data)!=11||strlen(tempo)!=8)
    return;

  sscanf(data, "%s %d %d", omes, &dia, &ano);
  sscanf(tempo, "%d:%d:%d", &hora, &min, &seg);
  mes=(strstr(nomes, omes)-nomes)/3+1;
//  sprintf(sversion,"%04d%02d%02d.%02d%02d%02d", ano, mes, dia, hora, min, seg);
//  fv=atof(sversion);
    snprintf(sversion,20," Ver %s(%04d%02d%02d)", VER, ano, mes, dia);
  return;
}
/* [] END OF FILE */
