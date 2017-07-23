/*
 * Handles slot listener and slot controls and events
 */


#include <xc.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "slot_task.h"

#define SLOT_OPTION_PASSWORD 1
#define SLOT_OPTION_FLASHY 2


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
            marquee("$$$ ", data->displayTickCount);
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

