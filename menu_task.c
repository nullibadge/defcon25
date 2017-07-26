/*
 * Handles menu listener and menu controls and events
 */


#include <xc.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "menu_task.h"
#include "slot_task.h"
#include "matrix_task.h"


#define MENU_OPTION_SLOT 1
#define MENU_OPTION_PASSWORD 2
#define MENU_OPTION_MATRIX 3
#define MENU_OPTION_FLASHY 4 //has to be last


char *menu_option[] = {
    "MENU",
    "SLOT ",
    "PASSWORD ",
    "MATRIX ",
    "EXIT ",
};
u16 menu_option_idx;
u16 menu_option_size;


// LEDs to show for menu
//u16 menu_options_leds[] = { LED_9, LED_8, LED_7, LED_6, LED_5, LED_4, LED_3, LED_2, LED_1, LED_0, LED_15, LED_14, LED_13, LED_12, LED_11 };
u16 menu_options_leds[] = { LED_9, LED_8, LED_7, LED_6, LED_5 };


void menu_Init(void *taskData) {
    struct t_menu_taskData *data = (struct t_menu_taskData *) taskData;

    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    
    /* Initialize application data */
    data->delayTickCount = MENU_DELAY_TICK_COUNT;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
    
    menu_option_idx = 0;
    
    menu_option_size = sizeof(menu_option)/sizeof(menu_option[0]);
}

void menu_Main (void *taskData) {
    struct t_menu_taskData *data = (struct t_menu_taskData *) taskData;
    u8 buttonPressed;
    
    if ( currentTask != MENU_TASK ){
        // master menu control
        buttonPressed = nullifyBadge_buttonsGet();
        switch (buttonPressed) {
            case BUTTON_A | BUTTON_B | BUTTON_Y:
                currentTask = MENU_TASK;
                menu_clear(data);
                
                /* Make sure lot task data is reset */
                slot_Setup(slot_taskData);
                
                // prevent holding X from escaping more than once
                data->holdCountA = 1;
                data->holdCountB = 1;
                data->holdCountX = 1;
                data->holdCountY = 1;

                menu_option_idx = 0;
                menu_display_option(data);
                break;
            default:
                // prevent holding X from escaping more than once
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
        case BUTTON_A:
            // confirm button isn't being held down
            if (data->holdCountA == 0){
                // move left
                if (menu_option_idx == 0){
                    menu_option_idx = menu_option_size - 1;
                }
                else {
                    menu_option_idx -= 1;
                }
            }
            menu_display_option(data);
            data->holdCountA += 1;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_B:
            // confirm button isn't being held down
            if (data->holdCountB == 0){
                // move left
                if (menu_option_idx == menu_option_size - 1){
                    menu_option_idx = 0;
                }
                else {
                    menu_option_idx += 1;
                }
            }
            menu_display_option(data);
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_X:
            if (data->holdCountX == 0){
                // exit
                menu_clear(data);
                currentTask = FLASHY_TASK;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX += 1;
            data->holdCountY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_Y:
            if (data->holdCountY == 0){
                menu_run_option(data);
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY += 1;
            data->noholdCount = 0;
            break;
        case BUTTON_A | BUTTON_B | BUTTON_Y:
        default:
            menu_display_option(data);
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount += 1;
    }
    
}

void menu_display_option(struct t_menu_taskData *data){
    u16 size = sizeof(menu_options_leds)/sizeof(menu_options_leds[0]);
    u16 led = menu_options_leds[menu_option_idx % size];
    
    // add lock base LEDs
    //led |= LED_4 | LED_3 | LED_2 | LED_1 | LED_0 | LED_15 | LED_14 | LED_13 | LED_12 | LED_11;

    //nullifyBadge_userLedsSet(data->badge, 0x0);
    nullifyBadge_userLedsSet(data->badge, led);

    if (menu_option_idx == 0){
        nullifyBadge_segDisplayPutStr(data->badge, menu_option[menu_option_idx]);
        nullifyBadge_segDisplayForceUpdate(data->badge);
    }
    else {
        marquee(menu_option[menu_option_idx], data->noholdCount);
    }
}

void menu_run_option(struct t_menu_taskData *data){
    switch (menu_option_idx){
        case MENU_OPTION_SLOT:
            menu_clear(data);
            currentTask = SLOT_TASK;
            break;
        case MENU_OPTION_PASSWORD:
            menu_clear(data);
            currentTask = LOCKED_TASK;
            break;
        case MENU_OPTION_FLASHY:
            menu_clear(data);
            currentTask = FLASHY_TASK;
            break;
        case MENU_OPTION_MATRIX:
            menu_clear(data);
            currentTask = MATRIX_TASK;
            break;
    }
}

void menu_clear(struct t_menu_taskData *data){
    /* Initialize application data */
    data->delayTickCount = MENU_DELAY_TICK_COUNT;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
    
    menu_option_idx = 0;
    
    // clear display and led
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
}

void menu_Exit(void *taskData) {
    struct t_menu_taskData *data = (struct t_menu_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware

    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
}