/**
  TMR4 Generated Driver File

  @Company
    Microchip Technology Inc.

  @File Name
    tmr4.c

  @Summary
    This is the generated driver implementation file for the TMR4 driver using MPLAB(c) Code Configurator

  @Description
    This source file provides APIs for TMR4.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15.3
        Device            :  PIC16LF18346
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "tmr4.h"

/**
  Section: Global Variables Definitions
*/

void (*TMR4_InterruptHandler)(void);

/**
  Section: TMR4 APIs
*/

void TMR4_Initialize(void)
{
    // Set TMR4 to the options selected in the User Interface

    // T4CKPS 1:64; T4OUTPS 1:16; TMR4ON off; 
    T4CON = 0x7B;

    // PR4 255; 
    PR4 = 0xFF;

    // TMR4 0; 
    TMR4 = 0x00;

    // Clearing IF flag.
    PIR2bits.TMR4IF = 0;

    // Start TMR4
    TMR4_StartTimer();
}

void TMR4_StartTimer(void)
{
    // Start the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 1;
}

void TMR4_StopTimer(void)
{
    // Stop the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 0;
}

uint8_t TMR4_ReadTimer(void)
{
    uint8_t readVal;

    readVal = TMR4;

    return readVal;
}

void TMR4_WriteTimer(uint8_t timerVal)
{
    // Write to the Timer4 register
    TMR4 = timerVal;
}

void TMR4_LoadPeriodRegister(uint8_t periodVal)
{
   PR4 = periodVal;
}

bool TMR4_HasOverflowOccured(void)
{
    // check if  overflow has occurred by checking the TMRIF bit
    bool status = PIR2bits.TMR4IF;
    if(status)
    {
        // Clearing IF flag.
        PIR2bits.TMR4IF = 0;
    }
    return status;
}
/**
  End of File
*/