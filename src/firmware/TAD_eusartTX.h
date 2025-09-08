/* 
 * @file TAD_eusartTX.h
 * @author alberto.marquillas & alba.falcon
 * @date 19 de febrer de 2025
 * @brief Definició de funcions per la comunicació UART asíncrona en el PIC18F4321.
 */

 #ifndef TAD_EUSART_H
 #define TAD_EUSART_H
 
 #include <xc.h>
 
 /**
  * @Pre El sistema ha d'estar inicialitzat correctament.
  * @Post Configura els registres necessaris per a la comunicació UART a 9600 bps.
  */
 void EusartTX_Init(void);
 
 /**
  * @Pre La comunicació UART ha d'estar inicialitzada.
  * @Post Retorna 1 si el buffer de transmissió està buit, 0 en cas contrari.
  */
 char eusartTx_pucEnviar();
 
 /**
  * @Pre La comunicació UART ha d'estar inicialitzada i el buffer de transmissió disponible.
  * @Post El caràcter especificat s'envia per UART.
  */
 void eusartTx_sendChar(char c);
 
 /**
  * @Pre La comunicació UART ha d'estar inicialitzada.
  * @Post Retorna 1 si hi ha un caràcter disponible per llegir, 0 en cas contrari.
  */
 char eusartRX_heRebut();
 
 /**
  * @Pre Ha d'haver un caràcter disponible per llegir.
  * @Post Retorna el caràcter rebut a través de UART.
  */
 char eusartRX_rebreChar();
 
 #endif /* TAD_EUSART_H */