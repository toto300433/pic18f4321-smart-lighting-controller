/* 
 * File:   tad_menu.h
 * Author: alberto.marquillas & alba.falcon
 *
 * Created on 20 de febrero de 2025, 10:33
 */


#include <xc.h>
#include <string.h>
#include "TAD_rfidCooperatiu.h"
#include "tad_timer.h"
static unsigned char estado, i, estadoDelay, i_delay, estatToCard = 0;
static unsigned char TimerRFID;

//Funcio delay_us
#define NUM_US 10


//Funcio ToCard
static unsigned char i_ToCard, n_ToCard, lastBits, backBits,tempBackBits = 0;
static unsigned char irqEn, waitIRq = 0x00;
static unsigned char *backLenPtr, *backDataPtr;
static char _status = MI_ERR;

static unsigned char _status_aux = 0;

//AnitColl
static unsigned char unLen;

//Motor
static unsigned char UID[16];
static unsigned char UID_ascii[10];  // 5 bytes * 2 caracteres + terminador '\0'



static unsigned char HiHaNouUser = 0;
//-------------- Private functions: --------------
void InitPortDirections(){
DIR_MFRC522_SO  = 1; 
DIR_MFRC522_SI  = 0; 
DIR_MFRC522_SCK = 0; 
DIR_MFRC522_CS  = 0; 
DIR_MFRC522_RST = 0;
}


void delay_us(char howMany){
    
    switch(estadoDelay){
        case 0:
            i_delay = 0;
            estadoDelay = 1;
            break;
        case 1:
            if(i_delay < (howMany*NUM_US)){
                i_delay++;
            } else {
                estadoDelay = 0;
            }
            break;
    }
}

unsigned char MFRC522_Rd(unsigned char Address){
   unsigned int i, ucAddr;
   unsigned int ucResult = 0;
   MFRC522_SCK = 0;
   MFRC522_CS = 0;
   ucAddr = ( (Address<<1)&0x7E)|0x80;

   for (i = 8; i > 0; i--){
      MFRC522_SI= ((ucAddr&0x80) == 0x80);
      MFRC522_SCK = 1;
      //delay_us(5);

      ucAddr <<= 1;
      MFRC522_SCK = 0;
      //delay_us(5);
   }

   for (i = 8; i > 0; i--){
      MFRC522_SCK = 1;
      //delay_us(5);
      ucResult <<= 1;   
      ucResult|= MFRC522_SO;
      MFRC522_SCK = 0;
      //delay_us(5);
   }

   MFRC522_CS = 1;
   MFRC522_SCK = 1;
   return ucResult;
}


void MFRC522_Wr(unsigned char Address, unsigned char value) {
  
   unsigned char i, ucAddr;
   MFRC522_SCK = 0;
   MFRC522_CS = 0;
   ucAddr = ( (Address<<1)&0x7E);
   for (i = 8; i > 0; i--) {
      MFRC522_SI= ( (ucAddr&0x80) == 0x80);
      MFRC522_SCK = 1;
      ucAddr <<= 1;
      delay_us(5);
      MFRC522_SCK = 0;
      delay_us(5);
   }

   for (i = 8; i > 0; i--){
      MFRC522_SI = ( (value&0x80) == 0x80);
      MFRC522_SCK = 1;
      value <<= 1;
      delay_us(5);
      MFRC522_SCK = 0;
      delay_us(5);
   }
   
   MFRC522_CS = 1;
   MFRC522_SCK = 1;
}



void MFRC522_Clear_Bit( char addr, char mask ){     
    unsigned char  tmp =0x0;
    tmp=MFRC522_Rd( addr ) ;
    MFRC522_Wr( addr,  tmp&~mask );   
}


void MFRC522_Set_Bit( char addr, char mask ){    
    unsigned char  tmp =0x0;
      tmp=MFRC522_Rd( addr ) ; 
     MFRC522_Wr( addr, tmp|mask );
}
void MFRC522_Reset()
{ 
   MFRC522_RST = 1;
   delay_us (1);
   MFRC522_RST = 0;
   delay_us (1);
   MFRC522_RST = 1;
   delay_us (1);
   MFRC522_Wr( COMMANDREG, PCD_RESETPHASE );
   delay_us (1);
}

void MFRC522_AntennaOn()
{                                              
    unsigned char stt;
    stt= MFRC522_Rd( TXCONTROLREG ) ;
    MFRC522_Set_Bit( TXCONTROLREG, 0x03 );
}
void MFRC522_AntennaOff()
{
MFRC522_Clear_Bit( TXCONTROLREG, 0x03 );                                          
}
void MFRC522_Init()     
{                 
    MFRC522_CS = 1; 
    MFRC522_RST = 1;
   
     MFRC522_Reset();       
     MFRC522_Wr( TMODEREG, 0x8D );      //Tauto=1; f(Timer) = 6.78MHz/TPreScaler
     MFRC522_Wr( TPRESCALERREG, 0x3E ); //TModeReg[3..0] + TPrescalerReg
     MFRC522_Wr( TRELOADREGL, 30 );
     MFRC522_Wr( TRELOADREGH, 0 ); 
     MFRC522_Wr( TXAUTOREG, 0x40 );    //100%ASK
     MFRC522_Wr( MODEREG, 0x3D );      // CRC valor inicial de 0x6363
       
  
     MFRC522_AntennaOff() ;            
     MFRC522_AntennaOn();
}



char MFRC522_ToCard( char command, char *sendData, char sendLen, char *backData, unsigned *backLen ) { 
    
    switch (estatToCard) {
        case 0:
            i_ToCard = 0;
            n_ToCard=0;
            switch (command){
                case PCD_AUTHENT: 
                    irqEn = 0x12;
                    waitIRq = 0x10;
                    break;
                case PCD_TRANSCEIVE:
                    irqEn = 0x77;
                    waitIRq = 0x30;
                    break;
                default:
                    break;
            }
            estatToCard = 1;
            break;
        case 1:
            MFRC522_Wr( COMMIENREG, irqEn | 0x80 );
            estatToCard = 2;
            break;
        case 2:
            MFRC522_Clear_Bit( COMMIRQREG, 0x80 );
            estatToCard = 3;
            break;
        case 3:
            MFRC522_Set_Bit( FIFOLEVELREG, 0x80 );
            estatToCard = 4;
            break;
        case 4:
            MFRC522_Wr( COMMANDREG, PCD_IDLE );
            estatToCard = 5;
            
            break;
        case 5:
            MFRC522_Wr( FIFODATAREG, sendData[i_ToCard] );
            i_ToCard++;                  
            if (i_ToCard >= sendLen) {
                estatToCard = 6;
            }

            break;
        case 6:
            MFRC522_Wr( COMMANDREG, command );
            estatToCard = 7;
            break;
        case 7:
            if (command == PCD_TRANSCEIVE) {
                MFRC522_Set_Bit( BITFRAMINGREG, 0x80 );
            }
            i_ToCard = 0xFFFF;
            estatToCard = 8;
            break;
        case 8:
            n_ToCard = MFRC522_Rd( COMMIRQREG );
            i_ToCard--;
            if ( i_ToCard && !(n_ToCard & 0x01) && !( n_ToCard & waitIRq ) ){
                estatToCard=8;
            }else{
               
                estatToCard = 9;
            }
            break;
        case 9:
            MFRC522_Clear_Bit( BITFRAMINGREG, 0x80 ); 
            estatToCard = 10; 
            break;
        case 10:            // if i !=0
            (i_ToCard != 0) ? (estatToCard = 11) : (estatToCard = 18);
            break;
        case 11:        // f( !( MFRC522_Rd( ERRORREG ) & 0x1B ) )
            if (!(MFRC522_Rd( ERRORREG ) & 0x1B)) {
                _status = MI_OK;
                estatToCard = 12;
            } else {
                _status = MI_ERR;
                estatToCard = 18;
            }
            break;
        case 12:    //if ( n & irqEn & 0x01 ) _status = MI_NOTAGERR; 
            if (n_ToCard & irqEn & 0x01) { _status = MI_NOTAGERR;} 
                estatToCard = 13;   
            break;
        case 13:        //if ( command == PCD_TRANSCEIVE )
            (command == PCD_TRANSCEIVE) ? (estatToCard = 14) : (estatToCard = 18);
            break;
        case 14:
            n_ToCard = MFRC522_Rd( FIFOLEVELREG );
            estatToCard = 15;
            break;
        case 15:
            lastBits = MFRC522_Rd( CONTROLREG ) & 0x07;
            estatToCard = 16;
            break;
        case 16:        // if (lastBits) *backLen = (n-1) * 8 + lastBits;
            if (lastBits) {
                *backLen = (n_ToCard - 1) * 8 + lastBits;   // No detecta card
 
            } else {
                *backLen = n_ToCard * 8;        // Detecta card   
                //PASSA PER AQUI UID
            }
            if (n_ToCard == 0) {
                n_ToCard = 1;
            } else if (n_ToCard > 16) {
                n_ToCard = 16;
            }
            i_ToCard=0;
            estatToCard = 17;
            break;
        case 17:    // for (i=0; i < n; i++) backData[i] = MFRC522_Rd( FIFODATAREG );

            backData[i_ToCard] = MFRC522_Rd( FIFODATAREG );
            i_ToCard++;
            if (i_ToCard >= n_ToCard) {
                backData[i_ToCard] = 0;
                estatToCard = 18;
            }
            break;
        case 18:
            estatToCard = 0;
            return _status;
    }
    return _status;
}


 
char MFRC522_Request( char reqMode, char *TagType ) {

    MFRC522_Wr( BITFRAMINGREG, 0x07 );
    TagType[0] = reqMode;
    _status_aux = MFRC522_ToCard( PCD_TRANSCEIVE, TagType, 1, TagType, &tempBackBits ); // Usar variable local

    if ( (_status_aux != MI_OK) || (tempBackBits != 0x10) ) {
        _status_aux = MI_ERR;
    }

    return _status_aux;
}


void MFRC522_CRC( char *dataIn, char length, char *dataOut ){
char i, n;
    MFRC522_Clear_Bit( DIVIRQREG, 0x04 );
    MFRC522_Set_Bit( FIFOLEVELREG, 0x80 );   
   
    
    for ( i = 0; i < length; i++ ) MFRC522_Wr( FIFODATAREG, *dataIn++ );  
       
    MFRC522_Wr( COMMANDREG, PCD_CALCCRC );
       
    i = 0xFF;
 
    do{
        n = MFRC522_Rd( DIVIRQREG );
        i--;
    } while( i && !(n & 0x04) );        //CRCIrq = 1
       
    dataOut[0] = MFRC522_Rd( CRCRESULTREGL );
    dataOut[1] = MFRC522_Rd( CRCRESULTREGM );       
}
//hibernation
void MFRC522_Halt()
{
  unsigned unLen;
  char buff[4];
 
  buff[0] = PICC_HALT;
  buff[1] = 0;
  MFRC522_CRC( buff, 2, &buff[2] );
  MFRC522_Clear_Bit( STATUS2REG, 0x80 );
  MFRC522_ToCard( PCD_TRANSCEIVE, buff, 4, buff, &unLen );
  MFRC522_Clear_Bit( STATUS2REG, 0x08 );
}



char MFRC522_AntiColl(char *serNum) {
    char i;
    char serNumCheck = 0;
    unsigned unLen;
    char cmdBuffer[2];  // Separate buffer for commands
    
    MFRC522_Wr(BITFRAMINGREG, 0x00);
    
    // Store command in separate buffer
    cmdBuffer[0] = PICC_ANTICOLL;
    cmdBuffer[1] = 0x20;
    
    MFRC522_Clear_Bit(STATUS2REG, 0x08);
    
    // Use cmdBuffer as send data, serNum as receive buffer
    _status = MFRC522_ToCard(PCD_TRANSCEIVE, cmdBuffer, 2, serNum, &unLen);
    
    if (_status == MI_OK) {
        // Verify checksum
        for (i = 0; i < 4; i++)
            serNumCheck ^= serNum[i];
        
        if (serNumCheck != serNum[4])
            _status = MI_ERR;
    }
    
    return _status;
}

char MFRC522_isCard( char *TagType ){
    if (MFRC522_Request( PICC_REQIDL, TagType ) == MI_OK)
        return 1;
    else
        return 0;
}

/*
char MFRC522_ReadCardSerial( unsigned char *str ) {  
    _status = MFRC522_AntiColl( (char *) str );
    str[5] = 0;
    if (_status == MI_OK)
      return 1;
    else
      return 0;
}*/


char MFRC522_ReadCardSerial(unsigned char *str) {
    _status = MFRC522_AntiColl((char *)str);
    str[5] = 0;  // Null terminator after 5 bytes of UID
    
    if (_status == MI_OK)
        return 1;
    else
        return 0;
}

//-------------- Public functions: --------------
void initRFID(){
    InitPortDirections();  //Aquesta funci? ?s coopertaiva)
    MFRC522_Init(); 
    TI_NewTimer(&TimerRFID);
}


static unsigned char flagEspera = 0;

void ReadRFID_Cooperatiu(){
    static char TagType;
    //static unsigned char estado = 0;
    static unsigned char index = 0;
    switch (estado) {
        case 0:  // Espera 1 segundo antes de intentar leer de nuevo
            if (TI_GetTics(TimerRFID) >= 1000) {  // 1000 ms =  1 segundos
                    _status = MI_ERR;
                    estado = 1;
            }

            break;

        case 1:  // Comenzar la detecci?n de tarjeta
            if (MFRC522_isCard(&TagType)) {
                estado = 2;
            } 
            break;

        case 2:  // Intentar leer el UID
            if (MFRC522_ReadCardSerial(UID)) {
                index = 0;  // Reiniciamos ?ndice de conversi?n
                estado = 3; // Pasamos al estado de conversi?n
            } 
            break;

        case 3:  // Convertir UID a HEX ASCII (un byte por iteraci?n)
            if (index < 5) {                          
                // Convertir el byte actual a HEX ASCII
                UID_ascii[index * 2] = "0123456789ABCDEF"[(UID[index] >> 4) & 0x0F];
                UID_ascii[index * 2 + 1] = "0123456789ABCDEF"[UID[index] & 0x0F];
                index++;  // Avanzar al siguiente byte
            } else {
                //UID_ascii[10] = '\0';  // Agregar terminador de string
                index = 0;  // Reiniciar ?ndice para env?o
                HiHaNouUser = 1;
                estado = 4;  // Pasamos al estado de env?o
            }
            break;
        case 4:
            HiHaNouUser = 0;
            MFRC522_Halt();
            TI_ResetTics(TimerRFID);  // Reiniciamos el temporizador para la pr?xima detecci?n
            estado = 0; // Volvemos al estado inicial
            break;
            
    }
}



unsigned char rfid_hiHaNouUID(){
    return HiHaNouUser;
}



void rfid_getUID(char *UID_FUNC){
    memcpy(UID_FUNC, UID_ascii, 10);
}