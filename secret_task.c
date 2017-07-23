/*
 * Handles secret input
 */


#include <xc.h>
#include <stdarg.h>
#include <stdio.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "secret_task.h"


// secret input arrays of buttons, BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y,
struct secret_input 
{
    // array of input values
    u8 inputs[20];
    
    // current index in inputs
    u16 idx;
    
    // variable to set (prefer global))
    u16 *variable;
    
    // value to store in variable (prefer global))
    u16 value;
};

//struct secret_input secret_input_test = ;

// set secret input
struct secret_input secret_inputs[] = {
    {
        // ^^vv<><>ba
        { BUTTON_A, BUTTON_A, BUTTON_B, BUTTON_B, BUTTON_A, BUTTON_B, BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y },

        // set to initial index of 0
        0,

        // test secret, (extern in nullifyBadge.h && set 0 in main.c && used in flashy_task))
        &secret_value,

        // which test to set
        SECRET_VALUE_TEST,
    },
};
 

void secret_Init(void *taskData) {
    struct t_secret_taskData *data = (struct t_secret_taskData *) taskData;

    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    
    /* Initialize application data */
    data->delayTickCount = SECRET_DELAY_TICK_COUNT;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
}

u8 secret_check(struct t_secret_taskData *data, u8 buttonPressed){
    u8 *inputs;
    u16 size = sizeof(secret_inputs) / sizeof(secret_inputs[0]);
    u16 currents_size;
    char hit_counter[6];
    
    for (u16 i = 0; i < size; i++){
        inputs = secret_inputs[i].inputs;

        if (buttonPressed == inputs[secret_inputs[i].idx]){
            currents_size = strlen((const char *)inputs);
            secret_inputs[i].idx++;

            if (secret_inputs[i].idx == currents_size) {
                // set index back to 0
                secret_inputs[i].idx = 0;
                
                // set variable
                *(secret_inputs[i].variable) = (u16) secret_inputs[i].value;

                return 1;
            }
        } else {
            // wrong input, set index to 0
            secret_inputs[i].idx = 0;
        }
    }
    return 0;
}

void secret_Main (void *taskData) {
    struct t_secret_taskData *data = (struct t_secret_taskData *) taskData;
    u8 buttonPressed, secret_found = 0;
    
    buttonPressed = nullifyBadge_buttonsGet();
    switch (buttonPressed) {
        case NO_BUTTON:
            // ignore not pressing a button... possibly at a timer to clear?
            if (data->delayTickCount == 0){
                data->delayTickCount = SECRET_DELAY_TICK_COUNT;
                secret_check(data, NO_BUTTON);
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount += 1;
            data->delayTickCount -= 1;
            break;
        case BUTTON_A:
            // confirm button isn't being held down
            if (data->holdCountA == 0){
                secret_found = secret_check(data, buttonPressed);
            }
            data->holdCountA += 1;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            data->delayTickCount = SECRET_DELAY_TICK_COUNT;
            break;
        case BUTTON_B:
            // confirm button isn't being held down
            if (data->holdCountB == 0){
                secret_found = secret_check(data, buttonPressed);
            }
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            data->delayTickCount = SECRET_DELAY_TICK_COUNT;
            break;
        case BUTTON_X:
            // confirm button isn't being held down
            if (data->holdCountX == 0){
                secret_found = secret_check(data, buttonPressed);
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX += 1;
            data->holdCountY = 0;
            data->noholdCount = 0;
            data->delayTickCount = SECRET_DELAY_TICK_COUNT;
            break;
        case BUTTON_Y:
            // confirm button isn't being held down
            if (data->holdCountY == 0){
                secret_found = secret_check(data, buttonPressed);
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY += 1;
            data->noholdCount = 0;
            data->delayTickCount = SECRET_DELAY_TICK_COUNT;
            break;
        default:
            // clear counters and global secret indexes
            secret_check(data, NO_BUTTON);
            secret_clear(data);
    }
    
    // clear data if found, only allow one secret input
    if (secret_found){
        secret_clear(data);
    }
}

void secret_clear(struct t_secret_taskData *data){
    /* Initialize application data */
    data->delayTickCount = SECRET_DELAY_TICK_COUNT;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
}

void secret_Exit(void *taskData) {
    struct t_secret_taskData *data = (struct t_secret_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware

    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
}

