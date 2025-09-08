/*
 * @file TAD_eusartTX.c
 * @author alberto.marquillas & alba.falcon
 * @date 19 de febrer de 2025
 * @brief Gestió de la comunicació UART en mode asíncron per al PIC18F4321.
 */

 #include <xc.h>
 #include "TAD_eusartTX.h"
 
 /**
  * @brief Inicialitza la comunicació UART a 9600 bps.
  * @details Configura els registres de l'EUSART per a comunicació asíncrona.
  */
 void EusartTX_Init(void) {
     SPBRG = 64; // Configuració per a 9600 bps
     SPBRGH = 0;
     TXSTAbits.BRGH = BAUDCONbits.BRG16 = TXSTAbits.SYNC = 0;     // Baixa velocitat
     TXSTAbits.TXEN = TRISCbits.TRISC6 = RCSTAbits.SPEN = TRISCbits.TRISC7 = RCSTAbits.CREN = 1; // TX 
 }
 
 /**
  * @brief Comprova si es pot enviar un nou caràcter per UART.
  * @return 1 si el registre de transmissió està buit, 0 en cas contrari.
  */
 char eusartTx_pucEnviar() {
     return TXSTAbits.TRMT;
 }
 
 /**
  * @brief Envia un caràcter per UART.
  * @param c Caràcter a enviar.
  */
 void eusartTx_sendChar(char c) {
     TXREG = c;
 }
 
 /**
  * @brief Comprova si s'ha rebut un nou caràcter per UART.
  * @return 1 si hi ha un caràcter disponible, 0 en cas contrari.
  */
 char eusartRX_heRebut() {
     return PIR1bits.RCIF;
 }
 
 /**
  * @brief Llegeix un caràcter rebut per UART.
  * @return Caràcter rebut.
  */
 char eusartRX_rebreChar() {
     return RCREG;
 }