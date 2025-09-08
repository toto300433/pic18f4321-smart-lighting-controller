/* 
 * @file TAD_teclat.h
 * @author alberto.marquillas & alba.falcon
 * @date 21 de febrer de 2025
 * @brief Definició de les funcions i macros per al control del teclat matricial.
 */

 #ifndef TAD_TECLAT_H
 #define TAD_TECLAT_H
 
 // Definició de les files
 #define F0 LATAbits.LATA0
 #define F1 LATAbits.LATA1
 #define F2 LATAbits.LATA2
 #define F3 LATAbits.LATA3
 
 // Definició de les columnes
 #define C1 PORTCbits.RC5
 #define C0 PORTBbits.RB0
 #define C2 PORTBbits.RB1
 // Definició de les columnes

 // Constants per a la detecció de tecles
 #define REBOTS 16
 #define SEGONS3 3000
 
 /**
  * @Pre El sistema ha d'estar inicialitzat correctament.
  * @Post Inicialitza el teclat matricial.
  */
 void initTeclat(void);
 
 /**
  * @Pre El teclat ha d'estar inicialitzat.
  * @Post Rota les files per detectar tecles premudes.
  */
// void teclat_rotaFiles(void);
 
 /**
  * @Pre Hi ha d'haver una tecla premuda.
  * @Post Retorna el caràcter corresponent a la tecla premuda.
  */
// char teclat_guardaTeclaTeclat(void);
 
 /**
  * @Pre El teclat ha d'estar inicialitzat.
  * @Post Retorna 1 si hi ha una tecla premuda, 0 en cas contrari.
  */
// char teclat_teclaPremuda(void);
 
 /**
  * @Pre Ha d'haver estat premuda una tecla.
  * @Post Retorna 1 si hi ha una tecla registrada, 0 en cas contrari.
  */
 char teclat_hiHaTecla(void);
 
 /**
  * @Pre Ha d'haver estat premuda una tecla.
  * @Post Retorna el caràcter de la tecla premuda.
  */
 char teclat_getTeclaPremuda(void);
 
 /**
  * @Pre El teclat ha d'estar inicialitzat.
  * @Post Gestiona l'estat del teclat matricial.
  */
 void motorTeclat(void);
 
 
char teclat_getTempsDeHash();
 #endif /* TAD_TECLAT_H */
 