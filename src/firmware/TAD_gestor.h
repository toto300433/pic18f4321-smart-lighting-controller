/* 
 * @file TAD_gestor.h
 * @author alberto.marquillas & alba.falcon
 * @date 24 de febrer de 2025
 * @brief Definició de les funcions per a la gestió d'usuaris i control d'accessos.
 */

 #ifndef TAD_GESTOR_H
 #define TAD_GESTOR_H
 
 #include <xc.h> // Inclou els arxius del processador
 
 #define MAX_USERS 3 // Nombre màxim d'usuaris registrats
 
 /**
  * @struct UsersInfo
  * @brief Estructura per a l'emmagatzematge de la informació dels usuaris.
  */
 typedef struct {
     unsigned char UID[10]; // Identificador únic de l'usuari (RFID)
     unsigned char confiLeds[6]; // Configuració personalitzada dels LEDs per a cada usuari
 } UsersInfo;
 
 /**
  * @Pre El sistema ha d'estar inicialitzat correctament.
  * @Post Inicialitza la base de dades d'usuaris amb UIDs predefinits.
  */
 void initGestor(void);
 
 
 /**
  * @Pre El gestor ha d'estar inicialitzat.
  * @Post Gestiona els estats del sistema d'usuaris i el control d'accessos.
  */
 void motorGestor(void);
 
 
unsigned char gestor_getIntensity(unsigned char flag, unsigned char led, unsigned char n_user);
 
 unsigned char gestor_hiHaLogout();


 unsigned char gestor_hiHaUserNoAutoritzat();

 unsigned char gestor_hiHaNouUser();
 
void gestor_getUIDActual(char *UID_funct, unsigned char flag, unsigned char n_user);

char gestor_getConnected();
 #endif /* TAD_GESTOR_H */
 