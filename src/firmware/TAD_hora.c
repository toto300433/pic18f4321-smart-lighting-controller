/*
 * @file TAD_hora.c
 * @author alberto.marquillas & alba.falcon
 * @date 26 de febrer de 2025
 * @brief Gestió del rellotge intern del sistema cooperatiu.
 */

 #include <xc.h>
 #include "tad_timer.h"
 #include "TAD_hora.h"
 #include "TAD_lcd.h"
 
 static unsigned char minuts, hores, segons, estat, i, canvi_hora = 0;
 static unsigned char timerHora;
 static unsigned char horaActual[5], hora[5];
 
 /**
  * @brief Inicialitza el sistema de comptatge d'hora.
  * @details Crea un temporitzador per gestionar el temps.
  */
 void initHora() {
     TI_NewTimer(&timerHora);
 }
 
 
 void hora_getHora(unsigned char *hora){
     hora[0] = horaActual[0];
     hora[1] = horaActual[1];
     hora[2] = horaActual[2];
     hora[3] = horaActual[3];
     hora[4] = horaActual[4];

 }
 void hora_setHora(unsigned char *numeros){
     horaActual[0] = numeros[0];
     horaActual[1] = numeros[1];
     horaActual[2] = numeros[2];
     horaActual[3] = numeros[3];
     horaActual[4] = numeros[4];
     canvi_hora++;
 }
 
 /**
  * @brief Motor de gestió de l'hora.
  * @details Controla l'actualització del comptatge de minuts i segons.
  */
 void motorHora() {
     switch (estat) {
         case 0:
             TI_ResetTics(timerHora);
             estat++;
             break;
             
         case 1:
             if (TI_GetTics(timerHora) > 2000) { //2000 -> 1s, //180000 -> 1minut
                 TI_ResetTics(timerHora);
                 estat++;
             }
             break;
             
         case 2:
             if(segons == 59){  
                 segons = 0;
                if (minuts == 59) {
                    minuts = 0;
                    if (hores == 59) {
                        hores = 0;
                    } else {
                        hores++;
                    }
                } else {
                    minuts++;
                }
             } else {
                 segons++;
             }
             estat++;
             break;
             
         case 3:
             if(canvi_hora){
                hora_getHora(horaActual);
                hores = (horaActual[0] - '0') *10 + (horaActual[1] -'0');
                minuts = (horaActual[3] -'0') *10 + (horaActual[4] -'0');
                segons = 0;
                canvi_hora--;
             } else {
                horaActual[0] = ((hores / 10) + '0');
                horaActual[1] = ((hores % 10) + '0');
                horaActual[2] = ':';
                horaActual[3] = ((minuts / 10) + '0');
                horaActual[4] = ((minuts % 10) + '0');                 
             }
             i = 0;
             estat++;
             break;
         case 4:                      
             
             if(i < 5){
                LcGotoXY(2 + i, 0);     
                LcPutChar(horaActual[i]);
                i++;
             }else{
                 estat = 0;
             }

             break;
     }
 }