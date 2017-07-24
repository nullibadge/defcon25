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
#ifndef NULLIFYBADGE_H
#define	NULLIFYBADGE_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "xc8_types.h"
#include "nullifyBadge_displayL2.h"
#include "nullifyBadge_displayL1.h"
#include "gamma_correction.h"


// task values for currentTask
enum t_tasks {
    MENU_TASK,
    SLOT_TASK,
    FLASHY_TASK,
    LOCKED_TASK,
};

// global variables
extern u8 badge_locked;
extern u16 currentTask;
extern void marquee(char* string, u16 count);
extern void marquee_speedy(char* string, u16 count, u16 multiplier);

void led_roundabout(u16 counter);
void led_curtain(u16 counter);
void led_rand(void);
void led_circle(u16 count);
void let_alternate(u16 count);

// global secrets
enum t_secret {
    SECRET_VALUE_INIT,
    SECRET_VALUE_TEST,
};
extern u16 secret_value;

#define LED_0 1
#define LED_1 2
#define LED_2 4
#define LED_3 8
#define LED_4 16
#define LED_5 32
#define LED_6 64
#define LED_7 128
#define LED_8 256
#define LED_9 512
#define LED_10 1024
#define LED_11 2048
#define LED_12 4096
#define LED_13 8192
#define LED_14 16384
#define LED_15 32768


#define NO_BUTTON 0
#define BUTTON_A 1
#define BUTTON_B 2
#define BUTTON_X 4
#define BUTTON_Y 8

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
// live documentation

enum t_hw_register {
    HW_SEG_DISPLAY,
    HW_USER_LEDS,
    HW_IR_TX,
    HW_EEPROM
};

enum t_userLed {
    LED_G0,
    LED_G1,
    LED_G2,
    LED_G3,
    LED_G4,
    LED_G5,
    LED_G6,
    LED_G7,
    LED_G8,
    LED_G9,
    LED_R0,
    LED_G10,
    LED_G11,
    LED_G12,
    LED_G13,
    LED_G14,
    LED_G15
};

/*
enum t_buttons {
    BUTTON_A,
    BUTTON_B, 
    BUTTON_X,
    BUTTON_Y
};
*/

struct t_nullifyBadge {
    struct t_nullifyDisplay_l2 display;
    u8 registeredHw;
};

/* Initializes Nullify Badge Version 1 Revision 1 DEFCON 2017 Platform */
void nullifyBadge_platformInit(struct t_nullifyBadge *badge);

/* 
 * -----------------------------------------------------------------------------
 * API Functions for Segment Display 
 * -----------------------------------------------------------------------------
 */

/* Registers Segment Display hardware for use */
s8 nullifyBadge_segDisplayRegister(struct t_nullifyBadge *badge);

/* Releases the Segment Display hardware from use */
void nullifyBadge_segDisplayRelease(struct t_nullifyBadge *badge);

/* Queries the Segment Display hardware to deem if in use or not*/
u8 nullifyBadge_segDisplayIsRegistered(struct t_nullifyBadge *badge);

/* Sets brightness of Segment Display */
void nullifyBadge_segDisplaySetBrightness(u8 brightness);

/* Left shifts a single character onto the display - does not update display */
void nullifyBadge_segDisplayPutChar (struct t_nullifyBadge *badge, char c);

/* Places a character to the display at desired position - does not update display */
void nullifyBadge_segDisplayPutCharPos (struct t_nullifyBadge *badge, char c, u8 pos);

/* Left shifts a single character ontot the display - updates the display*/
void nullifyBadge_segDisplayPrintChar (struct t_nullifyBadge *badge, char c);

/* Forces the display to update with current character buffer */
void nullifyBadge_segDisplayForceUpdate (struct t_nullifyBadge *badge);

/* Left shifts a character array onto the display - does not update - will only retain last 4 characters of string */
void nullifyBadge_segDisplayPutStr (struct t_nullifyBadge *badge, char *str);

/* Left shifts a character array onto the display - updates display - will only retain last 4 characters of string */
void nullifyBadge_segDisplayPrintStr (struct t_nullifyBadge *badge, char *str);

/* Erases the segment display - forces an update with space characters */
void nullifyBadge_segDisplayErase (struct t_nullifyBadge *badge);

/* 
 * -----------------------------------------------------------------------------
 * API Functions for User LEDs 
 * -----------------------------------------------------------------------------
 */
/* Registers User LED hardware for use */
s8 nullifyBadge_userLedsRegister (struct t_nullifyBadge *badge);

/* Releases the User LED hardware from use */
void nullifyBadge_userLedsRelease (struct t_nullifyBadge *badge);

/* Queries the User LED hardware to deem if in use or not*/
u8 nullifyBadge_userLedsAreRegistered(struct t_nullifyBadge *badge);

/* Sets brightness of User LEDs*/
void nullifyBadge_userLedsSetBrightness(u8 brightness);

/* Reads and returns current state of User LEDs */
u16  nullifyBadge_userLedsGet (struct t_nullifyBadge *badge);

/* Sets data to the User LED vector, excludes red LED */
void nullifyBadge_userLedsSet (struct t_nullifyBadge *badge, u16 userLedData);

/* Sets data to the User LED vector */
void nullifyBadge_userLedsSetForce (struct t_nullifyBadge *badge, u16 userLedData);

/* Clears all User LEDs */
void nullifyBadge_userLedsClr (struct t_nullifyBadge *badge);

/* 
 * -----------------------------------------------------------------------------
 * API Functions for buttons 
 * -----------------------------------------------------------------------------
 */
/* Reads the current state of all buttons and returns as a vector */
u8 nullifyBadge_buttonsGet();

/* Reads the current state Button A*/
u8 nullifyBadge_buttonAGet();

/* Reads the current state Button B*/
u8 nullifyBadge_buttonBGet();

/* Reads the current state Button X*/
u8 nullifyBadge_buttonXGet();

/* Reads the current state Button Y*/
u8 nullifyBadge_buttonYGet();

/* 
 * -----------------------------------------------------------------------------
 * API Functions for Infrared 38 kHz Receiver
 * -----------------------------------------------------------------------------
 */
/* Reads the current state of the IR receiver */
u8 nullifyBadge_irGet();

/*
 * TODO: Implement function to set IR tx carrier wave frequency or period
 * while maintaining the duty cycle time.  Otherwise the LED may burn out...
 */

/* 
 * -----------------------------------------------------------------------------
 * API Functions for Infrared Transmitter
 * -----------------------------------------------------------------------------
 */
/* Registers IR Tx hardware for use */
s8   nullifyBadge_irTxRegister(struct t_nullifyBadge *badge);

/* Releases the IR Tx hardware from use */
void nullifyBadge_irTxRelease(struct t_nullifyBadge *badge);

/* Queries the IR Tx hardware to deem if in use or not*/
u8   nullifyBadge_irTxIsRegistered(struct t_nullifyBadge *badge);

/* Sets the Infrared LED */
void nullifyBadge_irTxSet();

/* Clears the Infrared LED */
void nullifyBadge_irTxClr();

/* 
 * -----------------------------------------------------------------------------
 * API Functions for EEPROM
 * -----------------------------------------------------------------------------
 */
/* Registers EEPROM hardware for use */
s8   nullifyBadge_eepromRegister(struct t_nullifyBadge *badge);

/* Releases the EEPROM hardware from use */
void nullifyBadge_eepromRelease(struct t_nullifyBadge *badge);

/* Queries the EEPROM hardware to deem if in use or not*/
u8   nullifyBadge_eepromRegistered(struct t_nullifyBadge *badge);

/* Performs 8bit write to EEPROM */
void nullifyBadge_eepromWrite8bit(u16 addr, u8 data);

/* Performs 1void6bit write to EEPROM*/
void nullifyBadge_eepromWrite16bit(u16 addr, u16 data);

/* Performs 8bit read from EEPROM */
u8 nullifyBadge_eepromRead8bit(u16 addr);

/* Performs 16bit read from EEPROM */
u16 nullifyBadge_eepromRead16bit(u16 addr);






#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

    

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* NULLIFYBADGE_H */

