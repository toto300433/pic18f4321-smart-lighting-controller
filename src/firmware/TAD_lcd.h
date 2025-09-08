#ifndef LCTLCD_H
#define	LCTLCD_H

//
// ------------------------------------HARDWARE---AREA--------------------
// The connection is of 4 data bits (D4 a D7), and the signals 
// RS, R/W and E.
// In this example we have connected: 
//
// 	RS				to RB3
//	R/!W            to RB15
//  E				to RB5
//	D4				to RB6
//	D5				to RB7
//	D6				to RB8
//	D7				to RB9

// 	RS				to RD1
//	R/!W            to RD2
//  E				to RD3
//	D4				to RD4
//	D5				to RD5
//	D6				to RD6
//	D7				to RD7

//  It is important that the LCD signals D0..D3 are left unused and 
//  that we connect them to GND using resistors. 
//
// 	You can use any configuration that you want modifying the
//  the following defines accordingly
//

#include <xc.h>


#define SetD4_D7Sortida()		(TRISDbits.TRISD4 = TRISDbits.TRISD5 = TRISDbits.TRISD6 = TRISDbits.TRISD7 = 0)
#define SetD4_D7Entrada()		(TRISDbits.TRISD4 = TRISDbits.TRISD5 = TRISDbits.TRISD6 = TRISDbits.TRISD7 = 1)
#define SetControlsSortida()    (TRISDbits.TRISD3 = TRISDbits.TRISD2 = TRISDbits.TRISD1 = 0)
#define SetD4(On)				(LATDbits.LATD4 = (On))
#define SetD5(On)				(LATDbits.LATD5 = (On))
#define SetD6(On)				(LATDbits.LATD6 = (On))
#define SetD7(On)				(LATDbits.LATD7 = (On))
#define GetBusyFlag()           (PORTCbits.RC0)
#define RSUp()					(LATDbits.LATD1 = 1)
#define RSDown()				(LATDbits.LATD1 = 0)
#define RWUp()					(LATDbits.LATD2 = 1)
#define RWDown()				(LATDbits.LATD2 = 0)
#define EnableUp()				(LATDbits.LATD3 = 1)
#define EnableDown()            (LATDbits.LATD3 = 0)
// -------------------------------END--HARDWARE---AREA--------------------


void LcInit(char rows, char columns);
// Pre: Rows = {1, 2, 4}  Columns = {8, 16, 20, 24, 32, 40 }
// Pre: There is a free timer
// Pre: It needs 40ms of tranquility between the VCC raising and this constructor being called.
// Post: This routine can last up to 100ms
// Post: The display remains cleared, the cursor turned off and at the position 0, 0.

void LcGotoXY(char Column, char Row);
// Pre : Column between 0 and 39, row between 0 and 3. 
// Post: Sets the cursor to those coordinates. 
// Post: The next order can last up to 40us. 

void LcPutChar(char c);
// Post: Paints the char in the actual cursor position and increments 
// its position. If the column gets to 39 it returns to 0.
// The row of the LCD is increased when this happens until the second
// row and then it is reset back to row 0 if it has 2 rows total. 
// If the LCD has 4 rows it will reset back to row 0 when it
// reaches row 4 and the columns will go till 39 before reseting to 0.
// The one row LCDs returns to 0 when a column gets to 39. 
// The row is never increased. 
	// The char is written


#endif	/* LCTLCD_H */

