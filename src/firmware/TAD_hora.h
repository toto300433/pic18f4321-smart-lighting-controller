/* 
 * @file TAD_hora.h
 * @author alberto.marquillas & alba.falcon
 * @date 26 de febrer de 2025
 * @brief Definició de les funcions per la gestió del rellotge del sistema.
 */

 #ifndef TAD_HORA_H
 #define TAD_HORA_H
 
 /**
  * @Pre El sistema ha d'estar inicialitzat i el temporitzador disponible.
  * @Post Inicialitza el comptador de temps per gestionar minuts i segons.
  */
 void initHora(void);
 
 /**
  * @Pre El mòdul d'hora ha d'estar inicialitzat.
  * @Post Actualitza el comptatge de minuts i segons, i mostra l'hora al LCD.
  */
 void motorHora(void);
 
 void hora_setHora(unsigned char *numeros);
 void hora_getHora(unsigned char *hora);
 #endif /* TAD_HORA_H */
 