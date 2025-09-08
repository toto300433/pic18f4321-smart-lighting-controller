/*
 * @file main.c
 * @author alberto.marquillas & alba.falcon
 * @date 19 de febrer de 2025
 * @brief Programa principal per al control del sistema cooperatiu en PIC18F4321.
 */

 #include <xc.h>
 #include "tad_timer.h"
 #include "TAD_eusartTX.h"
 #include "TAD_menu.h"
 #include "TAD_rfidCoop.h"
 #include "TAD_lcd.h"
 #include "TAD_gestor.h"
 #include "TAD_hora.h"
 #include "TAD_teclat.h"
 #include "TAD_leds.h"

 // Configuració del microcontrolador
 #pragma config OSC = HSPLL
 #pragma config PBADEN = DIG
 #pragma config WDT = OFF
 #pragma config LVP = OFF

 /**
  * @brief Gestió de la interrupció d'alta prioritat.
  * @details Gestiona les interrupcions generades pel temporitzador TMR0.
  */
 extern void __interrupt (high_priority) HighRSI(void) {
     if (INTCONbits.TMR0IF == 1) {
         RSI_Timer0();
     }
 }

 /**
  * @brief Inicialitza tots els mòduls del sistema.
  * @details Crida a les funcions d'inicialització de cada TAD.
  */
 void Main_Init() {

    TI_Init();
    initRFID();
    initGestor();
    initLed();
    EusartTX_Init();
    initMenu();
    LcInit(2, 16);
    initHora();
    initTeclat();  

 }

 /**
  * @brief Funció principal del sistema.
  * @details Executa el bucle principal en mode cooperatiu.
  */
void main(void) {
    Main_Init(); // Inicialització del sistema

    while (1) {
        motorTeclat();
        ReadRFID_Cooperatiu();
        motorLed(0);
        motorLed(1);
        motorLed(2);
        motorLed(3);
        motorLed(4);
        motorLed(5); 
        motorHora();
        motorMenu();
        motorGestor();
    }
}
