/* 
 * @file TAD_leds.c
 * @author afalc
 * @date 27 de febrer de 2025
 * @brief GestiA?  de la intensitat dels LEDs i control d'estats.
 */

 #include "TAD_leds.h"
 #include "TAD_gestor.h"
 #include "tad_timer.h"
 // DefiniciA?  de la intensitat dels LEDs en percentatges
 static unsigned char intensity[11] = {0 , 2, 4, 6, 8 ,10, 12, 14, 16, 18, 20};

 static unsigned char led_tecla, tecla = 0; 
 static unsigned char TimerLed[6], estat[6];
 
 void initLed() {
    TI_NewTimer(&TimerLed[0]);     
    TI_NewTimer(&TimerLed[1]);
    TI_NewTimer(&TimerLed[2]);
    TI_NewTimer(&TimerLed[3]);
    TI_NewTimer(&TimerLed[4]);
    TI_NewTimer(&TimerLed[5]);
    TRISBbits.TRISB7 = 0;
    TRISBbits.TRISB6 = 0;
    TRISEbits.TRISE0 = 0;
    TRISBbits.TRISB4 = 0;
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB2 = 0;
    
    /*!!!!!!!!*/
    LATBbits.LATB7 = LATBbits.LATB6 = LATEbits.LATE0 = LATBbits.LATB4 = LATBbits.LATB3 = LATBbits.LATB2 = 0;
    estat[0] = estat[1] = estat[2] = estat[3] = estat[4] = estat[5] = 0;    
 }

 void led_valorTecla(unsigned char tecla, unsigned char valor) {
    switch(tecla) {
        case 0:
            LATBbits.LATB7 = valor;
            break;
        case 1:
            LATBbits.LATB6  = valor;
            break;
        case 2:
            LATEbits.LATE0  = valor;
            break;
        case 3:
            LATBbits.LATB4  = valor;
            break;
        case 4:
            LATBbits.LATB3  = valor;
            break;
        case 5:
            LATBbits.LATB2  = valor;
            break;
    }
 }
 
 void motorLed(unsigned char led) {
     if(gestor_getConnected()){
        switch(estat[led]) {
            case 0:
                if(TI_GetTics(TimerLed[led]) < ((20 - intensity[(gestor_getIntensity(2, led, 0) - '0')]))*2) { //20tics = 20ms (Priode led))
                    led_valorTecla(led, 0);
                } else {
                    estat[led]++;
                    TI_ResetTics(TimerLed[led]);
                }
                break;
            case 1:

                if(TI_GetTics(TimerLed[led]) < ((20 - (20 - intensity[(gestor_getIntensity(2, led, 0) - '0')])))*2) { //20tics = 20ms (Priode led))
                    led_valorTecla(led, 1);
                } else {
                    TI_ResetTics(TimerLed[led]);
                    estat[led]--;
                }
                break;
         } 
     } else {
        led_valorTecla(led, 0);
     }
     
 }