/* 
 * @file TAD_gestor.c
 * @author alberto.marquillas & alba.falcon
 * @date 24 de febrer de 2025
 * @brief Gesti? dels usuaris i control d'accessos mitjan?ant RFID.
 */



#include <xc.h>
#include <string.h>

#include "TAD_gestor.h"
#include "TAD_lcd.h"
#include "TAD_rfidCooperatiu.h"
#include "tad_timer.h"
#include "TAD_leds.h"
#include "TAD_teclat.h"

static UsersInfo userInfo[MAX_USERS];
static char i = 0;
static unsigned char estat = 10;
static unsigned char UID_actual[10] = "**********";
static unsigned char n_userConnected = 100; //Posicio del struct de l'usuari connectat
static unsigned char connected = 0, k;
static unsigned char userLogout = 0;
static unsigned char index, userNoAutoritzat, n_Led, intensitatLed, newUser = 0; //0 = no hi ha ningu a la sala; 1 = hi ha alg? a la sala

static unsigned char iteracions = 0;
/**
 * @brief Inicialitza la base de dades d'usuaris.
 * @details Configura els UIDs i les configuracions inicials dels LEDs.
 */

 

void initGestor(){
    userInfo[0].UID[0] = '1'; userInfo[0].UID[1] = '4'; userInfo[0].UID[2] = '6';
    userInfo[0].UID[3] = '0'; userInfo[0].UID[4] = '0'; userInfo[0].UID[5] = '5';
    userInfo[0].UID[6] = 'A'; userInfo[0].UID[7] = '7'; userInfo[0].UID[8] = 'D';
    userInfo[0].UID[9] = '6'; userInfo[0].UID[10] = '\0';

    userInfo[0].confiLeds[0] = '0'; userInfo[0].confiLeds[1] = '0'; userInfo[0].confiLeds[2] = '0';
    userInfo[0].confiLeds[3] = '0'; userInfo[0].confiLeds[4] = '0'; userInfo[0].confiLeds[5] = '0';
    userInfo[0].confiLeds[6] = '\0';

    userInfo[1].UID[0] = 'C'; userInfo[1].UID[1] = '5'; userInfo[1].UID[2] = '8';
    userInfo[1].UID[3] = '8'; userInfo[1].UID[4] = '1'; userInfo[1].UID[5] = 'B';
    userInfo[1].UID[6] = '2'; userInfo[1].UID[7] = 'F'; userInfo[1].UID[8] = '7';
    userInfo[1].UID[9] = '9'; userInfo[1].UID[10] = '\0';

    userInfo[1].confiLeds[0] = '0'; userInfo[1].confiLeds[1] = '0'; userInfo[1].confiLeds[2] = '0';
    userInfo[1].confiLeds[3] = '0'; userInfo[1].confiLeds[4] = '0'; userInfo[1].confiLeds[5] = '0';
    userInfo[1].confiLeds[6] = '\0';

    userInfo[2].UID[0] = '1'; userInfo[2].UID[1] = '3'; userInfo[2].UID[2] = 'A';
    userInfo[2].UID[3] = '3'; userInfo[2].UID[4] = 'F'; userInfo[2].UID[5] = '4';
    userInfo[2].UID[6] = 'D'; userInfo[2].UID[7] = '9'; userInfo[2].UID[8] = '9';
    userInfo[2].UID[9] = 'D'; userInfo[2].UID[10] = '\0';

    userInfo[2].confiLeds[0] = '0'; userInfo[2].confiLeds[1] = '0'; userInfo[2].confiLeds[2] = '0';
    userInfo[2].confiLeds[3] = '0'; userInfo[2].confiLeds[4] = '0'; userInfo[2].confiLeds[5] = '0';
    userInfo[2].confiLeds[6] = '\0';
}




/*
void gestor_resetUIDActual(){
    UID_actual[0] = '*';
    UID_actual[1] = '*';
    UID_actual[2] = '*';
    UID_actual[3] = '*';
    UID_actual[4] = '*';
    UID_actual[5] = '*';
    UID_actual[6] = '*';
    UID_actual[7] = '*';
    UID_actual[8] = '*';
    UID_actual[9] = '*';
    UID_actual[10] = '\0';
}*/


void gestor_ActualitzaUIDNoRegistrat(unsigned char i) {
    memcpy(UID_actual, userInfo[i].UID, 10);
}

/**
 * @brief Comprova si un UID est? registrat.
 * @param i ?ndex de l'usuari a verificar.
 * @return 1 si el UID coincideix, 0 en cas contrari.
 */
unsigned char gestor_buscaUIDreg(unsigned char i) {
    return (memcmp(UID_actual, userInfo[i].UID, 10) == 0) ? 1 : 0;
}

/**
 * @brief Actualitza l'estat del gestor en funci? de les noves deteccions RFID.
 */
void gestor_actualitzaEstat(){
    if(rfid_hiHaNouUID()){
        estat = 0;
    }   
}

char gestor_getConnected(){ return connected;}



/**
 * @brief Motor de gesti? del sistema d'usuaris.
 * @details Gestiona els estats de connexi?, verificaci? i configuraci? d'usuaris.
 */
void motorGestor(){
   // initGestor();
    gestor_actualitzaEstat();
    switch (estat){
        case 10:
            break;
        case 0:
            newUser = 0;
            rfid_getUID(UID_actual);
            if(connected){
                estat = 1;             
            } else {
                index = 0;
                estat++;
                estat++;
            }
            break;
            
        case 1:
            
            //Comprovar si el nou UID es l'usuari que hi ha a la sala
            if(gestor_buscaUIDreg(n_userConnected)){
                //Si es el mateix usuari vol dir que ha fet logout
                //Activar flags newUser i userLogout perque les rebi el menu
                newUser = 1;
                userLogout = 1;
                //Usuari ha marxat de la sala
                connected = 0;
                estat = 50;

            } else {
                
                index = 0;
                estat++;
                connected = 1;
                //ha entrat un nou usuari a la sala -> mirar si el nou usuari existeix al struct
            }
                        
            break;
            
        case 50:
            if(iteracions < 10){
                //UID_actual[0] = '*';
                iteracions++;
            }
            
            UID_actual[10] = '\0';
            estat = 3;
            break;
            
        case 2:
            
            //mirar si el nou usuari existeix al struct
            if(index < MAX_USERS){
                if(gestor_buscaUIDreg(index)){
                    //si es troba el nou usuari -> actualitzar 'punter' del struct
                    //activar flags que utilitzar? el menu
                    newUser = 1;
                    userNoAutoritzat = 0;

                    n_userConnected = index;
                    connected = 1;
                    estat++;
                    
                }
                index++;

            } else {
                
                newUser = 1;
                
                userNoAutoritzat = 1;
                gestor_ActualitzaUIDNoRegistrat(n_userConnected);             
                estat = 3;
            }
            break;
            
        case 3:
            //Desctivar flags del menu perque no siguin detectades
            newUser = 0;
            userLogout = 0;
            LcGotoXY(0, 0);
            if(!userNoAutoritzat) LcPutChar(UID_actual[9]);
            n_Led = 0;
            i = 0;
            userNoAutoritzat = 0;
            estat = 30;
            break;
        case 5:
            if(teclat_hiHaTecla()){
                n_Led = teclat_getTeclaPremuda();

                if(teclat_getTempsDeHash() == 1){
                    estat = 25;
                    i = 0;
                    
                }else if (connected){
                    estat++;
                }
            }
            
            break;
        case 25:
            if(k < 3){
                if(i < 6){
                    userInfo[k].confiLeds[i] = '0';
                    i++;
                } else {
                    i = 0;
                    n_Led = 0;
                    k++;
                }
            }else{
                estat = 30;
            }
            break;
        case 6:
            if(teclat_hiHaTecla()){
                intensitatLed = teclat_getTeclaPremuda();
                userInfo[n_userConnected].confiLeds[n_Led - '0'] = (intensitatLed == '*') ? 'A' : intensitatLed;
                n_Led = 0;
                i = 0;
                estat = 30;
            }
            break; 
        case 30:
            if(n_Led < 6){
                if(i < 8){
                    LcGotoXY(8+i, 0);
                }else {
                    LcGotoXY(i - 8, 1);
                }
                LcPutChar((n_Led) + '0');
                LcPutChar('-');
                LcPutChar(connected ? userInfo[n_userConnected].confiLeds[n_Led] : '*');
                LcPutChar(' ');
                n_Led++;
                i+=4;

            }else{
                estat = 5;
                k = 0;
            }

            
            break;
    }
}

unsigned char gestor_getIntensity(unsigned char flag, unsigned char led, unsigned char n_user) { 
    if(flag == 2){
        if(userInfo[n_userConnected].confiLeds[led] == 'A'){
            return ':';
        } else {
            return userInfo[n_userConnected].confiLeds[led];
        }
    } else if (flag == 1){
        return userInfo[n_user].confiLeds[led];
    } else if(flag == 0) {
        return userInfo[n_userConnected].confiLeds[led];
    }
}


unsigned char gestor_hiHaUserNoAutoritzat() { return userNoAutoritzat; }
unsigned char gestor_hiHaNouUser() { return newUser; }



void gestor_getUIDActual(char *UID_funct, unsigned char flag, unsigned char n_user) {
    if(flag){
        memcpy(UID_funct, userInfo[n_user].UID, 10);
    } else {
        memcpy(UID_funct, UID_actual, 10);

    }
}
unsigned char gestor_hiHaLogout() { return userLogout; }
