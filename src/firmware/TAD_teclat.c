/* 
 * @file TAD_teclat.c
 * @author alberto.marquillas & alba.falcon
 * @date 21 de febrer de 2025
 * @brief Gestió del teclat matricial i detecció de tecles.
 */


#include <pic18f4321.h>
#include "TAD_teclat.h"
#include "tad_timer.h"

static unsigned char estat, filaPremuda, timerTeclat, teclaPremuda, tempsHash, hiHaTecla = 0;
static unsigned long ticsHash = 0;
static const unsigned char teclas[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

/**
 * @brief Inicialitza el teclat matricial.
 * @details Configura les entrades i sortides i inicialitza el temporitzador.
 */
void initTeclat(){
    TI_NewTimer(&timerTeclat);
    // Configurar filas como salidas (0)
    TRISAbits.TRISA0 = TRISAbits.TRISA1 = TRISAbits.TRISA2 = TRISAbits.TRISA3 = 0;  

    // Configurar columnas como entradas (1)
    TRISBbits.TRISB0 = TRISCbits.TRISC5 = TRISBbits.TRISB1 = 1; 
}

/**
 * @brief Rota les files del teclat per escanejar tecles premudes.
 */
void teclat_rotaFiles() {
    //incrementar ?ndex i manternir-ho menor a 4
    filaPremuda = (filaPremuda + 1) % 4;
    PORTA = (PORTA & 0xF0) | (1 << filaPremuda);
}


/**
 * @brief Guarda la tecla premuda.
 * @return Caràcter corresponent a la tecla premuda.
 */
char teclat_guardaTeclaTeclat(){
    if(C0){
        return teclas[filaPremuda][0];
    }
    if(C1){
        return teclas[filaPremuda][1];
    }
    if(C2){
        return teclas[filaPremuda][2];
    }
}

/**
 * @brief Comprova si hi ha una tecla premuda.
 * @return 1 si hi ha tecla, 0 en cas contrari.
 */
char teclat_teclaPremuda(){
    if(C0 || C1 || C2) return 1;
    return 0;
}

/**
 * @brief Retorna el temps de pulsació de la tecla #.
 * @return Temps registrat per la tecla #.
 */
char teclat_getTempsDeHash(){return tempsHash;}

/**
 * @brief Comprova si hi ha una tecla premuda.
 * @return 1 si hi ha tecla, 0 en cas contrari.
 */
char teclat_hiHaTecla(){return hiHaTecla;}

/**
 * @brief Retorna la tecla premuda actual.
 * @return Caràcter de la tecla premuda.
 */
char teclat_getTeclaPremuda(){return teclaPremuda;}

/**
 * @brief Motor de gestió del teclat.
 * @details Controla l'estat del teclat i gestiona les pulsacions.
 */
void motorTeclat(){
    switch (estat){
        case 0:
            teclat_rotaFiles();
            estat++;
            hiHaTecla = ticsHash =  0;
            break;
        case 1:
            if(teclat_teclaPremuda()){
                //S'ha premut una tecla -> Gestionar rebots
                TI_ResetTics(timerTeclat);
                estat++;
            }else {
                estat--;
            }
        case 2:
            //Control rebots entrada
            if(TI_GetTics(timerTeclat) > REBOTS){
                if(teclat_teclaPremuda()){
                    teclaPremuda = teclat_guardaTeclaTeclat();
                    estat++;
                } else {
                    estat = 0;
                }
            }
            break;
            
        case 3:
            //Es queda en aquest estat fins que no es deixa de p?mer la tecla
            if(!teclat_teclaPremuda()){
                ticsHash = TI_GetTics(timerTeclat);
                TI_ResetTics(timerTeclat);
                (teclaPremuda == '#') ? (estat = 5) : estat++;
            } else {
            //Si no hi ha tecla -> Pasar al seguent estat (rebots sortida)
                tempsHash = 0;
            }
            
            break;
        case 4:
            if(TI_GetTics(timerTeclat) > REBOTS){
                if(!teclat_teclaPremuda()){hiHaTecla = 1;}    
                estat = 0;
            }
            break;
        case 5:
            (ticsHash > SEGONS3) ? (tempsHash = 1) : (tempsHash = 2);
            TI_ResetTics(timerTeclat);
            estat--;
            break;
    }
}