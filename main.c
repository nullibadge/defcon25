/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using MPLAB(c) Code Configurator

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC16LF18346
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.42
        MPLAB             :  MPLAB X 3.65
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
#include <stdio.h>
#include <pic16f18346.h>
#include "xc8_types.h"

#include "nullifyBadge.h"
#include "cooperative_scheduler.h"

#include "flashy_task.h"
#include "menu_task.h"
#include "slot_task.h"
#include "locked_task.h"
#include "secret_task.h"
#include "matrix_task.h"


/*
                         Main application
 */

/* Declare Badge Hardware Instance */
struct t_nullifyBadge badge_inst;


/* Define available tasks */
static struct t_schedulerTask flashy_task;
static struct t_schedulerTask menu_task;
static struct t_schedulerTask slot_task;
static struct t_schedulerTask locked_task;
static struct t_schedulerTask secret_task;
static struct t_schedulerTask matrix_task;


/* Allocate task data structures */
struct t_flashy_taskData flashy_TaskData;
struct t_menu_taskData menu_TaskData;
struct t_slot_taskData slot_TaskData;
struct t_locked_taskData locked_TaskData;
struct t_secret_taskData secret_TaskData;
struct t_matrix_taskData matrix_TaskData;


struct t_scheduler schedulerInst;

// global task state
u16 currentTask;
u8 badge_locked;
u16 secret_value;
u16 oddeven;


// Cycle LED arrays
u16 roundabout_leds[] = { LED_7, LED_8, LED_9, LED_11, LED_12, LED_13, LED_14, LED_15, LED_14, LED_13, LED_12, LED_11, LED_9, LED_8 };
u16 roundabout_leds_inverse[] = { LED_7, LED_6, LED_5, LED_4, LED_3, LED_2, LED_1, LED_0, LED_1, LED_2, LED_3, LED_4, LED_5, LED_6 };

u16 curtain_leds[] ={
    LED_7,
    LED_7 | LED_8,
    LED_7 | LED_8 | LED_9,
    LED_7 | LED_8 | LED_9 | LED_11,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12 | LED_13,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12 | LED_13 | LED_14,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12 | LED_13 | LED_14 | LED_15,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12 | LED_13 | LED_14,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12 | LED_13,
    LED_7 | LED_8 | LED_9 | LED_11 | LED_12,
    LED_7 | LED_8 | LED_9 | LED_11,
    LED_7 | LED_8 | LED_9,
    LED_7 | LED_8,
};
u16 curtain_leds_inverse[] ={
    LED_7,
    LED_7 | LED_6,
    LED_7 | LED_6 | LED_5,
    LED_7 | LED_6 | LED_5 | LED_4,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3 | LED_2,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3 | LED_2 | LED_1,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3 | LED_2 | LED_1 | LED_0,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3 | LED_2 | LED_1,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3 | LED_2,
    LED_7 | LED_6 | LED_5 | LED_4 | LED_3,
    LED_7 | LED_6 | LED_5 | LED_4,
    LED_7 | LED_6 | LED_5,
    LED_7 | LED_6,
};


void marquee_speedy(char* string, u16 count, u16 multiplier) {
    u16 string_length = strlen((const char *) string);
    char display_string[] = { ' ', ' ', ' ', ' ', '\0' };
    u16 i = (count / multiplier) % string_length;

    for (u16 j=0; j < 4; j++){
        display_string[j] = string[(i + j) % string_length];
    }
    nullifyBadge_segDisplayPutStr(&badge_inst, display_string);
    nullifyBadge_segDisplayForceUpdate(&badge_inst);
}


void marquee(char* string, u16 count) {
    u16 multiplier = 50;
    u16 string_length = strlen((const char *) string);
    char display_string[] = { ' ', ' ', ' ', ' ', '\0' };
    u16 i = (count / multiplier) % string_length;

    for (u16 j=0; j < 4; j++){
        display_string[j] = string[(i + j) % string_length];
    }
    nullifyBadge_segDisplayPutStr(&badge_inst, display_string);
    nullifyBadge_segDisplayForceUpdate(&badge_inst);
}

void led_roundabout(u16 counter) {
    u16 size = sizeof(roundabout_leds) / sizeof(roundabout_leds[0]);
    u16 leds = roundabout_leds[counter % size];

    size = sizeof(roundabout_leds_inverse) / sizeof(roundabout_leds_inverse[0]);
    leds |= roundabout_leds_inverse[counter % size];

    nullifyBadge_userLedsSet(&badge_inst, leds);
}

void led_curtain(u16 counter) {
    u16 size = sizeof(curtain_leds) / sizeof(curtain_leds[0]);
    u16 leds = curtain_leds[counter % size];

    size = sizeof(curtain_leds_inverse) / sizeof(curtain_leds_inverse[0]);
    leds |= curtain_leds_inverse[counter % size];

    nullifyBadge_userLedsSet(&badge_inst, leds);
}

void led_rand(void){
    //random number between 
    u16 spark = rand() % 65535;
    if ( (spark | 64511) == 65535 ){
        spark ^= 1024 ;
    }
    nullifyBadge_userLedsSet(&badge_inst,spark);        
}

void led_circle(u16 count){
    
    nullifyBadge_userLedsSet(&badge_inst,count);       
    
}

void let_alternate(u16 count){
    if(count %2 == 1){
        nullifyBadge_userLedsSet(&badge_inst,43349);       
    }
    else{
        nullifyBadge_userLedsSet(&badge_inst,21162);       
    }
}

void led_Parody(u16 count){
    u16 size; 
    u16 leds;
    
    if (count % 30 == 0){
        oddeven ^= 1 ;
    }

    if(count %2 == oddeven){

        size = sizeof(roundabout_leds) / sizeof(roundabout_leds[0]);
        leds = roundabout_leds[count % size];
    }
    else{
        size = sizeof(roundabout_leds_inverse) / sizeof(roundabout_leds_inverse[0]);
        leds = roundabout_leds_inverse[count % size];
    }
    nullifyBadge_userLedsSet(&badge_inst, leds);
}



void main(void)
{
    /* Initialize the platform */
    nullifyBadge_platformInit(&badge_inst);
    
    /* Initialize Cooperative Scheduler */
    scheduler_Init(&schedulerInst);

    /* Initialize tasks */
    scheduler_TaskInit(&flashy_task);
    scheduler_TaskInit(&menu_task);
    scheduler_TaskInit(&slot_task);
    scheduler_TaskInit(&locked_task);
    scheduler_TaskInit(&secret_task);
    scheduler_TaskInit(&matrix_task);
    
    /* Pass external data to task data structures */
    flashy_TaskData.badge      = &badge_inst;
    menu_TaskData.badge      = &badge_inst;
    slot_TaskData.badge      = &badge_inst;
    locked_TaskData.badge      = &badge_inst;
    secret_TaskData.badge      = &badge_inst;
    matrix_TaskData.badge      = &badge_inst;

    
    // set badge to locked state (red light on)
    badge_locked = 1;
    
    /* Configure Tasks */
    flashy_task.taskData = (void *) &flashy_TaskData;
    flashy_task.taskInitCallback = flashy_Init;
    flashy_task.taskExecuteCallback = flashy_Main;
    flashy_task.taskExitCallback = flashy_Exit;
    flashy_task.taskInterval = 10;
    
    menu_task.taskData = (void *) &menu_TaskData;
    menu_task.taskInitCallback = menu_Init;
    menu_task.taskExecuteCallback = menu_Main;
    menu_task.taskExitCallback = menu_Exit;
    menu_task.taskInterval = 10;
    
    slot_task.taskData = (void *) &slot_TaskData;
    slot_task.taskInitCallback = slot_Init;
    slot_task.taskExecuteCallback = slot_Main;
    slot_task.taskExitCallback = slot_Exit;
    slot_task.taskInterval = 10;
    
    locked_task.taskData = (void *) &locked_TaskData;
    locked_task.taskInitCallback = locked_Init;
    locked_task.taskExecuteCallback = locked_Main;
    locked_task.taskExitCallback = locked_Exit;
    locked_task.taskInterval = 10;
    
    secret_task.taskData = (void *) &secret_TaskData;
    secret_task.taskInitCallback = secret_Init;
    secret_task.taskExecuteCallback = secret_Main;
    secret_task.taskExitCallback = secret_Exit;
    secret_task.taskInterval = 10;
    
    matrix_task.taskData = (void *) &matrix_TaskData;
    matrix_task.taskInitCallback = matrix_Init;
    matrix_task.taskExecuteCallback = matrix_Main;
    matrix_task.taskExitCallback = matrix_Exit;
    matrix_task.taskInterval = 10;    
    
    // initialize secrets
    secret_value = SECRET_VALUE_INIT;
    
    /* Launch Tasks */
    currentTask = FLASHY_TASK;
    //currentTask = MATRIX_TASK;
    scheduler_TaskAdd(&schedulerInst, &flashy_task);
    scheduler_TaskAdd(&schedulerInst, &menu_task);
    scheduler_TaskAdd(&schedulerInst, &slot_task);
    scheduler_TaskAdd(&schedulerInst, &locked_task);
    scheduler_TaskAdd(&schedulerInst, &secret_task);
    scheduler_TaskAdd(&schedulerInst, &matrix_task);

    
    while(1){
        schedulerExecute(&schedulerInst);
    }
    
}
    




    
/**
 End of File
*/

