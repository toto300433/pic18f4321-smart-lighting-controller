/*
 * @file TAD_menu.c
 * @author alberto.marquillas & alba.falcon
 * @date 20 de febrer de 2025
 * @brief Gesti√≥ del men√∫ del sistema via UART.
 */

#include <xc.h>
#include "TAD_eusartTX.h"
#include "TAD_menu.h"
#include "TAD_gestor.h"
#include "TAD_hora.h"
#include "TAD_teclat.h"
#include <string.h>

static char menuX, menuY, estat, opcioRebuda, caracter, flagRebut, indexUID, i, j, userNoAutoritzat, flag_leds, n_User = 0;
unsigned char posicioLed = 5;
unsigned char logout = 1;
static char UID_menu[10];
static const unsigned char tarjetaDetectada[20] = "\r\nTarjeta detectada!";
static const unsigned char UIDMessage[7] = "\r\nUID: ";
static const unsigned char MessageUserNoAutoritzat[20] = "\r\nUser no autoritzat";
static const unsigned char menuMessages[7][32] = {
    "\r\n---------------              \n", // index 0
    "\rMenu principal                \n", // 1
    "\r---------------               \n", // 2
    "\rTria una opcio:              \n\n", // 3
    "\r\t1. Qui hi ha a la sala?      \n", // 4
    "\r\t2. Mostrar configuracions    \n", // 5
    "\r\t3. Modificar hora del sistema\n"  // 6
};

/*
1. Qui hi ha a la sala?: Mostrar l?UID de l?usuari que hi ha actualment a la sala. Si no hi ha
ning?, caldr? indicar-ho tamb?.
2. Mostrar configuracions: Mostrar la configuraci? dels llums per cadascun dels UIDs que
hi ha registrats al sistema.
3. Modificar hora del sistema: Permetre modificar l?hora del sistema. Sempre que es
reinici? la placa, l?hora del sistema s?haur? de posar a les 00:00.*/

static const unsigned char opcioMessage[9] = "\r\nOpcio: ";
static const unsigned char errorMessage[33] = "\n\rERROR: Tria una opcio de 1 a 3\n";
static unsigned char noUIDMessage[26] = "\n\rNo hi ha ningu a la sala";
static unsigned char horaMessage[21] = "\n\rIntrodueix l'hora: ";
static char LedsConfig[6];
static unsigned char numeros[5] = {0, 0, 0, 0, 0};
static char missatgeLeds[33] = {'\n', '\r', 'L', '0', ':', '0', 45, 'L', '1', ':', '0', 45, 'L', '2', ':', '0', 45, 'L', '3', ':', '0', 45, 'L', '4', ':', '0', 45, 'L', '5', ':', '0', '\n'};
//Config leds a posicions 6, 11, 16, 21, 26, 31 ;
/**
 * @brief Inicialitza el men√∫.
 */
void initMenu(){
}

char menu_enviaCharMenu(unsigned char n_trama){
    if(eusartTx_pucEnviar() == 1){
        if(n_trama == 0){ 
            eusartTx_sendChar('\n');
        }
        
        if(n_trama == 1){
            eusartTx_sendChar(menuMessages[menuY][menuX]);
        }
        
        if(n_trama == 2){
            eusartTx_sendChar(opcioMessage[menuX]);
        }
        if(n_trama == 3){
            eusartTx_sendChar(errorMessage[menuX]);
        }
        if(n_trama == 4){
            eusartTx_sendChar(noUIDMessage[menuX]);
        }
        if(n_trama == 5){
            eusartTx_sendChar(horaMessage[menuX]);
        }
        if(n_trama == 7){
            eusartTx_sendChar(tarjetaDetectada[menuX]);
        }
        if(n_trama == 8){
            eusartTx_sendChar(UIDMessage[menuX]);
        }
        if(n_trama == 11){
            eusartTx_sendChar(MessageUserNoAutoritzat[menuX]);
        }
        if(n_trama == 12){
            eusartTx_sendChar(UID_menu[menuX]);
        }
        if(n_trama == 13){
            eusartTx_sendChar(missatgeLeds[menuX]);
        }
    
        return 1;
    } else {
        return 0; //No s'ha pogut enviar
    }
}

/**
 * @brief Rep i reenvia un car√†cter via UART.
 * @return 1 si s'ha rebut correctament, 0 en cas contrari.
 */
char menu_rebCharMenu(){
    if(eusartRX_heRebut() && eusartTx_pucEnviar()){       
        opcioRebuda = eusartRX_rebreChar();
        if(eusartTx_pucEnviar()){
            eusartTx_sendChar(opcioRebuda);
            return 1;
        } 
    }
    return 0;
}


char menu_sendInfoCompleta(unsigned char maxArray, unsigned char n_array){
    if(menuX < maxArray){   
        if(menu_enviaCharMenu(n_array)){
            menuX++;
        } 
        return 0;
    }
    menuX = 0;
    return 1;
}



void menu_ActualitzaEstat(){
    
    
    if(gestor_hiHaNouUser()){
        userNoAutoritzat = gestor_hiHaUserNoAutoritzat();        
        logout = gestor_hiHaLogout();
        if(!logout){
            gestor_getUIDActual(UID_menu, flag_leds, 0);
        }

        //menuX = 0;
        estat = 9;

    }
    if(menu_rebCharMenu()){
        flagRebut = 1;
        opcioRebuda = eusartRX_rebreChar();
        if(opcioRebuda== 0x1B){
            menuY = 0;
            estat = 1;
        }
    }
    
    //usuari ha apretat el hashtag durant mes de tres segons
    if(teclat_getTempsDeHash() == 2 && teclat_hiHaTecla()){
        estat = 0;
    }   
}

/**
 * @brief Motor del men√∫, gestiona les interaccions i mostra informaci√≥.
 */
void motorMenu(){    
    menu_ActualitzaEstat();
    
    switch (estat){
        case 0: 
            
            flagRebut=menuX= 0;
            //Si s'ha acabat de printar el menu -> estat 2
            //Si encara falten linies del men˙ per printar -> Estat 1
            if(menuY == 7){
                menuY = 0;
                estat = 2;         
            } else {
                estat++;
            }      
            break;

        case 1:
            //Enviar fila del men˙ principal a la Eusart
            if(menu_sendInfoCompleta(32, 1)){estat--; menuY++;}; 
            break;

        case 2:
            //Enviar el missatge de "Opcio:"
            if(menu_sendInfoCompleta(9, 2)){estat++;}
            break;

        case 3:
            //Seleccionar un nou estat en funciÛ de la opciÛ que hagi introduit l'usuari
            if(flagRebut && opcioRebuda != 0x1B){           
                if(opcioRebuda == '1'){ 
                    estat++;
                } else if(opcioRebuda == '2'){
                    flag_leds = 1;
                    estat = 5;
                } else if(opcioRebuda == '3'){
                    estat = 6;
                } else {
                    estat = 7;
                } 
                flagRebut= 0;
            }
            
            break;
        case 4:
            //OPCIO 1 del men˙
            //Mirar si hi ha algun usuari a la sala
            if(!gestor_getConnected()) {
                //No hi ha cap usuari a la sala -> Missatge de que no hi ha ning˙
                if(menu_sendInfoCompleta(26, 4)){estat=17;};
            } else {
                //Hi ha usuari a la sala -> Printar el UID (guardat a UID_menu)
                if(menu_sendInfoCompleta(10, 12)){
                    menu_enviaCharMenu(0);
                    estat=17;
                }
            }         
            break;
        case 5:
            gestor_getUIDActual(UID_menu, flag_leds, n_User);
            estat = 11;

            //1. Mostra el UDI[n_User]
            //2. Ves a estat 50 i mostra LEDs
            //3. Torna fins que n_User <50

            break;
        case 6:
            //OPCI” 3 del menu
            //Prinat missatge de introduir la hora
            if(menu_sendInfoCompleta(21, 6)){estat=25;};    
            break;
        case 25:
            //Agafar la nova hora que ha introduit l'usuari pel teclat
            if(flagRebut && opcioRebuda != 0x1B){ 
                numeros[menuX] = opcioRebuda;
                if(menuX < 4) {
                    menuX++;
                    flagRebut = 0;

                } else {
                    hora_setHora(numeros);
                    i = 0;
                    estat = 17;
                }
            }
            break;
            
        case 7:
            //Estat que printa el missatge d'error quan s'ha introduit una opciÛ incorrecta
            if(menu_sendInfoCompleta(33, 3)){estat=2;};    //Torna a l'estat que printa opciÛ
            break;
           
        case 9:
            //Entra a aquest estat quan s'ha detectat un nou UID
            if(menu_sendInfoCompleta(20, 7)){     //Printa missatge "tarjeta detectada!"
                (userNoAutoritzat) ? estat = 12 : estat++;
                posicioLed = 5;
            }
            break;
        case 10:
            if(menu_sendInfoCompleta(7, 8)){j = 0; estat++;};    //Printa missatge "UID: "
            break;
            
        case 11:
            if (indexUID < 10) {  
                while(!eusartTx_pucEnviar());
                if (eusartTx_pucEnviar()) {
                    eusartTx_sendChar(UID_menu[indexUID]);  // Enviar un car?cter
                    indexUID++;  // Avanzar al siguiente car?cter
                }
                if((indexUID == 2) || (indexUID == 4) || (indexUID == 6) || (indexUID == 8)){
                    while(!eusartTx_pucEnviar());
                    if (eusartTx_pucEnviar()) {
                        eusartTx_sendChar('-');  // Enviar un car?cter
                    }
                }
            }else {                
                indexUID = 0;
                estat = 50;
                posicioLed = 5;
                //Hauria d'anar a un estat que printi la config dels LEDs
            }
            
            break;
            
        case 50:
            if(menuX < 6){    
                if(flag_leds){
                    missatgeLeds[posicioLed] = gestor_getIntensity(flag_leds, menuX, n_User);
                } else {
                    missatgeLeds[posicioLed] = gestor_getIntensity(flag_leds, menuX, 0);
                }               
                posicioLed += 5;
                menuX++;

            } else {
                estat = 51;
                menuX=0;
                posicioLed = 5;
            }            
            break;
        case 51:
            //printar configuraciÛ dels leds
            if(menu_sendInfoCompleta(32, 13)){
                if(flag_leds){
                    if(n_User < 2){
                        estat = 5;
                        n_User++;
                    } else {
                        n_User = 0;
                        flag_leds = 0;
                        estat = 17;
                    }
                    //es queda aqui esperant o s'enva a l'estat 17
                } else {
                    estat = 0;
                }
            }
            
            break;
        case 12:
            //Estat per printar la cadena de user no autoritzat
            if(menu_sendInfoCompleta(20, 11)){estat=16;};    
            break;
            
        case 16:
            if(menu_enviaCharMenu(0)){
                indexUID = 0;
                estat = 0;
            }

            break;
        case 17:
            //menuY = menuX = 0;
            //s'espera en aquest estat fins que es prem # o ESC o es detecta un nou UID
            break;
    }
    
}
