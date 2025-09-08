/* 
 * @file TAD_menu.h
 * @author alberto.marquillas & alba.falcon
 * @date 20 de febrer de 2025
 * @brief Definició de les funcions del menú del sistema.
 */

 #ifndef TAD_MENU_H
 #define TAD_MENU_H
 
 /**
  * @Pre El sistema ha d'estar inicialitzat correctament.
  * @Post Inicialitza el menú del sistema.
  */
 void initMenu(void);
 
 /**
  * @Pre El menú ha d'estar inicialitzat.
  * @Post Executa el motor del menú gestionant les opcions de l'usuari.
  */
 void motorMenu(void);
 
 /**
  * @Pre Ha d'existir una opció vàlida triada.
  * @Post Retorna l'opció seleccionada per l'usuari per depuració.
  */
// char menu_debugOpcioTriada(char opcio);
 
 
 /**
  * @Pre La comunicació UART ha d'estar operativa.
  * @Post Envia un caràcter del menú via UART.
  */
// char menu_enviaCharMenu(unsigned char n_trama);
 
 /**
  * @Pre La comunicació UART ha d'estar operativa.
  * @Post Retorna un caràcter rebut via UART.
  */
// char menu_rebCharMenu(void);
 
 #endif /* TAD_MENU_H */
 