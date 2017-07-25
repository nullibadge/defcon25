/*
 * Handles slot listener and slot controls and events
 */


#include <xc.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "slot_task.h"

#define SLOT_OPTION_PASSWORD 1
#define SLOT_OPTION_FLASHY 2

/* Globals */
u8 Slot_Wheel[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L',  \
'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', \
'2','3', '4', '5', '6', '7', '8', '9', '<', '>', '!', '$', '%', '&', '-', '#', \
'*', '+', '-', '.', '@', ';', ':', '\'', '"', '/', '\\'};
u8 *Shot_Options[] = {"SHOT", "BEER", "VDKA", "WSKY", "TQLA", "CHUG", "REBL", "FIRE", "WATR"};
u8 *Shot_Options_Long[] = {"SHOT   ", "BEER   ", "VODKA   ", "WHISKEY   ", "TEQUILA   ", "CHUG   ", "RED BULL   ", "FIRE BALL   ", "WATER   "};
u8 Shot_Index = 0;
u16 Leds_idx = 1;
extern struct t_slot_taskData* slot_taskData;

void slot_Init(void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;

    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    
    /* Initialize application data */
    data->displayTickCount = 0;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
    slot_Setup(data);
}

void slot_Setup(void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;
    slot_taskData = data;
    data->displayTickCount = 0;
    data->delayTickCount = 0;
    memset(data->slot_print_buf, 0x00, PRINT_BUFF_SIZE);
    if (!badge_locked) {
        Shot_Index = (u8)rand() % (sizeof(Shot_Options)/sizeof(Shot_Options[0]));
    }
    else {
        Shot_Index = ((sizeof(Shot_Options)/sizeof(Shot_Options[0]))-1);
    }
    
    Leds_idx = 1;
}

void slot_Running_Animation(void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;
    if (data->delayTickCount ==0 ){
      if (Leds_idx == 1024 ){
          Leds_idx = Leds_idx << 1;
      }
      led_circle(Leds_idx);
      if (Leds_idx < 1 ){
          Leds_idx = 1;
      }
      Leds_idx = Leds_idx << 1;

      data->delayTickCount = 10;
  }
  else {
      data->delayTickCount -= 1;
  }
    
}

void slot_Finished_Animation(void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;
    if (data->delayTickCount < 1){
        led_rand();
        data->delayTickCount = 10;
    }
    else {
        data->delayTickCount -= 1;
    }
    
}

void slot_Main (void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;
    u8 buttonPressed;
    
    // prevent task from running if not current task
    if ( currentTask != SLOT_TASK ){
        buttonPressed = nullifyBadge_buttonsGet();
        switch (buttonPressed) {
            default:
                // prevent holding from escaping more than once
                data->holdCountA = 1;
                data->holdCountB = 1;
                data->holdCountX = 1;
                data->holdCountY = 1;
                break;
        }
        return;
    }
    
    buttonPressed = nullifyBadge_buttonsGet();
    switch (buttonPressed) {
        default:
            /* Clear buffer */
            memset(data->slot_print_buf, 0x00, PRINT_BUFF_SIZE);
            
			/* Update the wheel every cycles */
			if (data->displayTickCount < WHEEL0_CYCLES) { 
                slot_Running_Animation(data);
				data->slot_wheel_index[0] = (u8)rand() % strlen(Slot_Wheel); 
				data->slot_wheel_index[1] = (u8)rand() % strlen(Slot_Wheel); 
				data->slot_wheel_index[2] = (u8)rand() % strlen(Slot_Wheel);
		        data->slot_wheel_index[3] = (u8)rand() % strlen(Slot_Wheel);
                sprintf(data->slot_print_buf, "%c%c%c%c", Slot_Wheel[data->slot_wheel_index[0]], Slot_Wheel[data->slot_wheel_index[1]], Slot_Wheel[data->slot_wheel_index[2]], Slot_Wheel[data->slot_wheel_index[3]]);
                marquee(data->slot_print_buf, 1);} 
		    else if (data->displayTickCount < WHEEL1_CYCLES) {  
                slot_Running_Animation(data);
				data->slot_wheel_index[1] = (u8)rand() % strlen(Slot_Wheel);  
				data->slot_wheel_index[2] = (u8)rand() % strlen(Slot_Wheel);
		        data->slot_wheel_index[3] = (u8)rand() % strlen(Slot_Wheel);
                sprintf(data->slot_print_buf, "%c%c%c%c", Shot_Options[Shot_Index][0], Slot_Wheel[data->slot_wheel_index[1]], Slot_Wheel[data->slot_wheel_index[2]], Slot_Wheel[data->slot_wheel_index[3]]);
                marquee(data->slot_print_buf, 1);}
		    else if (data->displayTickCount < WHEEL2_CYCLES) {
                slot_Running_Animation(data);
				data->slot_wheel_index[2] = (u8)rand() % strlen(Slot_Wheel);
		        data->slot_wheel_index[3] = (u8)rand() % strlen(Slot_Wheel);
                sprintf(data->slot_print_buf, "%c%c%c%c", Shot_Options[Shot_Index][0], Shot_Options[Shot_Index][1], Slot_Wheel[data->slot_wheel_index[2]], Slot_Wheel[data->slot_wheel_index[3]]);
                marquee(data->slot_print_buf, 1);} 
		    else if (data->displayTickCount < WHEEL3_CYCLES) { 
                slot_Running_Animation(data);
		        data->slot_wheel_index[3] = (u8)rand() % strlen(Slot_Wheel);
                sprintf(data->slot_print_buf, "%c%c%c%c", Shot_Options[Shot_Index][0], Shot_Options[Shot_Index][1], Shot_Options[Shot_Index][2], Slot_Wheel[data->slot_wheel_index[3]]);
                /* Marquee the wheel */
                marquee(data->slot_print_buf, 1);}
            else if (data->displayTickCount < FINAL_CYCLE) {
                slot_Finished_Animation(data);
                if ( data->displayTickCount % 50 <= 25 ) {
                    memset(data->slot_print_buf, 0x20, 4);
                }
               else {
                    sprintf(data->slot_print_buf, "%c%c%c%c", Shot_Options[Shot_Index][0], Shot_Options[Shot_Index][1], Shot_Options[Shot_Index][2], Shot_Options[Shot_Index][3]);
                }
                 marquee(data->slot_print_buf, 1);
            }
		    else {
                slot_Finished_Animation(data);
                if ( data->displayTickCount % 50 <= 25 ) {
                    memset(data->slot_print_buf, 0x20, 4);
                }
               else {
                    sprintf(data->slot_print_buf, "%s", Shot_Options_Long[Shot_Index]);
                }
                /* Marquee the wheel */
                 marquee_speedy(data->slot_print_buf, data->displayTickCount, 20);
                if (data->displayTickCount == FINAL_CYCLE + (50 * 10)) {
                     currentTask = MENU_TASK;
               
                    /* Make sure lot task data is reset */
                    slot_Setup(slot_taskData);
                
                    // prevent holding X from escaping more than once
                    data->holdCountA = 1;
                    data->holdCountB = 1;
                    data->holdCountX = 1;
                    data->holdCountY = 1;
                    break;
                }
		    }

            data->displayTickCount += 1;
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount += 1;
    }
    
}

void slot_Exit(void *taskData) {
    struct t_slot_taskData *data = (struct t_slot_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware

    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
}

