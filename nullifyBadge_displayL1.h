/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef NULLIFYBADGE_DISPLAYL1_H
#define	NULLIFYBADGE_DISPLAYL1_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "xc8_types.h"
#include "mcc_generated_files/mcc.h"
#include <string.h>

// TODO Insert appropriate #include <>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations

// Comment a function and leverage automatic documentation with slash star star
/**
    <p><b>Function prototype:</b></p>
  
    <p><b>Summary:</b></p>

    <p><b>Description:</b></p>

    <p><b>Precondition:</b></p>

    <p><b>Parameters:</b></p>

    <p><b>Returns:</b></p>

    <p><b>Example:</b></p>
    <code>
 
    </code>

    <p><b>Remarks:</b></p>
 */
// TODO Insert declarations or function prototypes (right here) to leverage 

/*
Creating characters:
bottom left = 0x1
top left = 0x100
bottom right = 0x80
top right = 0x4000
bottom =  0x20
top = 0x2000
dot = 0x40
left bottom diag = 0x2
right bottom diag = 0x8
left top diag = 0x200
right top diag = 0x800
middle bottom = 0x4
middle top = 0x400
left middle = 0x1000
right middle = 0x10
Example:
Char 7: 0x2000 or 0x4000 or 0x80 = 0x6080
*/

u16 asciiCharTableLookup[128] = {
  0x0000,
<<<<<<< HEAD
<<<<<<< HEAD
=======
  0x2000,
  0x1010,
  0x0020,
  0x1010,
  0x21A5, // 8 right
  0x60A5, // 8 left
  0x3010, // = high
  0x1200, // > high
  0x0810, // < high
  0x2020, // = wide
  0x4585, // triple |
  0x30B1, // 8 no top wall
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x0000, //  
  0x40C0, // !
  0x4400, // "
  0x1010, // #
  0x35B4, // $
  0x1010, // %
  0x1010, // &
  0x0400, // '
  0x0808, // (
  0x0202, // )
  0x1E1E, // *
  0x1414, // +
  0x0040, // ,
  0x1010, // -
  0x0040, // .
  0x0802, // /
  0x63A9, // 0
  0x4080, // 1
  0x7031, // 2
  0x60B0, // 3
  0x5190, // 4
  0x31B0, // 5
  0x31B1, // 6
  0x6080, // 7
  0x71B1, // 8
  0x7190, // 9
  0x1010, // :
  0x1010, // ;
  0x0808, // <
  0x1030, // =
  0x0202, // >
  0xE114, // ?
  0x70A5, // @
  0x7191, // A
  0x11B1, // B
  0x2121, // C
  0x50B1,
  0x3121,
  0x3101,
  0x21B1,
  0x5191,
  0x2424,
  0x40A1,
  0x1909,
  0x0121,
  0x4B81,
  0x4389,
  0x61A1,
  0x7111,
  0x61A9,
  0x7119,
  0x31B0,
  0x2404,
  0x41A1,
  0x0903,
  0x418B,
  0x0A0A,
  0x0A04,
  0x2822,
  0x2121,
  0x0208,
  0x60A0, // ]
  0x000A, // ^
  0x0020, // _
  0x1010, // `
  0x7191, // a
  0x11B1, // b
  0x1031, // c
u16 asciiCharTableLookup[128] = {
  0x0000,
>>>>>>> parent of 9256d1d... fix table
  0x2000, // slot1
  0x1010, // slot2
  0x0020, // slot3
  0x21A5, // balls right
  0x60A5, // balls left
  0x1200, // Tip top right
  0x0810, // Tip top left
  0x2020, // Wide Equals
  0x30B1, // 8 no top wall - balls
  0x4585, // triple pipe
  0x3010, // Top equals
  0x1010, 
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
=======
  0x2000,
  0x1010,
  0x0020,
  0x1010,
  0x21A5, // 8 right
  0x60A5, // 8 left
  0x3010, // = high
  0x1200, // > high
  0x0810, // < high
  0x2020, // = wide
  0x4585, // triple |
  0x30B1, // 8 no top wall
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x1010,
  0x0000, //  
  0x40C0, // !
  0x4400, // "
  0x1010, // #
  0x35B4, // $
  0x1010, // %
  0x1010, // &
  0x0400, // '
  0x0808, // (
  0x0202, // )
  0x1E1E, // *
  0x1414, // +
  0x0040, // ,
  0x1010, // -
  0x0040, // .
  0x0802, // /
  0x63A9, // 0
  0x4080, // 1
  0x7031, // 2
  0x60B0, // 3
  0x5190, // 4
  0x31B0, // 5
  0x31B1, // 6
  0x6080, // 7
  0x71B1, // 8
  0x7190, // 9
  0x1010, // :
  0x1010, // ;
  0x0808, // <
  0x1030, // =
  0x0202, // >
  0xE114, // ?
  0x70A5, // @
  0x7191, // A
  0x11B1, // B
  0x2121, // C
  0x50B1,
  0x3121,
  0x3101,
  0x21B1,
  0x5191,
  0x2424,
  0x40A1,
  0x1909,
  0x0121,
  0x4B81,
  0x4389,
  0x61A1,
  0x7111,
  0x61A9,
  0x7119,
  0x31B0,
  0x2404,
  0x41A1,
  0x0903,
  0x418B,
  0x0A0A,
  0x0A04,
  0x2822,
  0x2121,
  0x0208,
  0x60A0, // ]
  0x000A, // ^
  0x0020, // _
  0x1010, // `
  0x7191, // a
  0x11B1, // b
  0x1031, // c
  0x50B1,
  0x3121,
  0x3101,
  0x21B1,
  0x5191,
  0x2424,
  0x40A1,
  0x1909,
  0x0121,
  0x4B81,
  0x4389,
  0x61A1,
  0x7111,
  0x61A9,
  0x7119,
  0x31B0,
  0x2404,
  0x41A1,
  0x0903,
  0x418B,
  0x0A0A,
  0x0A04,
  0x2822,
>>>>>>> parent of 1755688... Update nullifyBadge_displayL1.h
  0x1010,
  0x0404, // |
  0x1010, // }
  0x1010, // ~
  0xFFFF
};

/* Level 1 Display API */
struct t_nullifyDisplay_l1 {
    u8 displayVect[10];
};

void displayL1_Initialize (struct t_nullifyDisplay_l1 *display);
void displayL1_SetSegChar (struct t_nullifyDisplay_l1 *display, char c, u8 charPos);
void displayL1_SetUserLed (struct t_nullifyDisplay_l1 *display, u16 userLedVect);
void displayL1_SetDp (struct t_nullifyDisplay_l1 *display, u8 displayDpVect);
void displayL1_Update (struct t_nullifyDisplay_l1 *display);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* NULLIFYBADGE_DISPLAYL1_H */

