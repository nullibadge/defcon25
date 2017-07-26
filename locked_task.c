/*
 * Handles core locked/unlock functionality
 */

#include <xc.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "locked_task.h"
#include "slot_task.h"


// LEDs located at base of lock
u16 locked_lock_base_leds = LED_4 | LED_3 | LED_2 | LED_1 | LED_0 | LED_15 | LED_14 | LED_13 | LED_12 | LED_11;

// LEDs located at top of lock
u16 locked_locking_leds[] = {
    0,
    LED_5,
    LED_5 | LED_6,
    LED_5 | LED_6 | LED_7,
    LED_5 | LED_6 | LED_7 | LED_8,
    LED_5 | LED_6 | LED_7 | LED_8 | LED_9,
};
u16 locked_locking_leds_idx;
u16 locked_leds;
u8 pw_found;
struct locked_secret_input 
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


// set secret input
struct locked_secret_input locked_secret_inputs[] = {
    {
        // ^^vv<><>ba
        { BUTTON_Y, BUTTON_B, BUTTON_Y, BUTTON_A },

        // set to initial index of 0
        0,

        // test secret, (extern in nullifyBadge.h && set 0 in main.c && used in flashy_task))
        &secret_value,

        // which test to set
        SECRET_VALUE_TEST,
    },
};

// forces LEDs to be in locked state
u16 locked_lock_leds(){
    u16 size = sizeof(locked_locking_leds) / sizeof(locked_locking_leds[0]);
    locked_locking_leds_idx = size - 1;
    locked_leds = locked_lock_base_leds | locked_locking_leds[locked_locking_leds_idx];
    return locked_leds;
}

// forces LEDs to be in unlocked state
u16 locked_unlock_leds(){
    locked_locking_leds_idx = 0;
    locked_leds = locked_lock_base_leds | locked_locking_leds[locked_locking_leds_idx];
    return locked_leds;
}

u16 locked_get_lock_leds(){
    u16 size = sizeof(locked_locking_leds) / sizeof(locked_locking_leds[0]);
    u16 leds = locked_locking_leds[locked_locking_leds_idx];
    
    // check if completely locked
    if (locked_locking_leds_idx < size - 1){
        locked_locking_leds_idx += 1;
    }
    return leds;
}

u16 locked_get_unlock_leds(){
    u16 leds = locked_locking_leds[locked_locking_leds_idx];
    
    // check if completely unlocked
    if (locked_locking_leds_idx > 0){
        locked_locking_leds_idx -= 1;
    }
    return locked_lock_base_leds | leds;
}





void locked_Init(void *taskData) {
    struct t_locked_taskData *data = (struct t_locked_taskData *) taskData;
    
    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    pw_found =0;
    /* Initialize application data */
    locked_lock_leds();
    data->state = LOCKED_LOCKED_STATE;
    data->delayTickCount = 0;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
}

u8 locked_secret_check(struct t_locked_taskData *data, u8 buttonPressed){
    u8 *inputs;
    u16 size = sizeof(locked_secret_inputs) / sizeof(locked_secret_inputs[0]);
    u16 currents_size;
    char hit_counter[6];
    
    for (u16 i = 0; i < size; i++){
        inputs = locked_secret_inputs[i].inputs;

        if (buttonPressed == inputs[locked_secret_inputs[i].idx]){
            currents_size = strlen((const char *)inputs);
            locked_secret_inputs[i].idx++;

            if (locked_secret_inputs[i].idx == currents_size) {
                // set index back to 0
                locked_secret_inputs[i].idx = 0;
                
                // set variable
                *(locked_secret_inputs[i].variable) = (u16) locked_secret_inputs[i].value;

                return 1;
            }
        } else {
            // wrong input, set index to 0
            locked_secret_inputs[i].idx = 0;
        }
    }
    return 0;
}


void locked_Main (void *taskData) {
    struct t_locked_taskData *data = (struct t_locked_taskData *) taskData;
    u8 buttonPressed;
    pw_found =0;
    if ( currentTask != LOCKED_TASK ){
        data->holdCountA = 1;
        data->holdCountB = 1;
        data->holdCountX = 1;
        data->holdCountY = 1;
        return;
    }
    
    // add password/auth here

    // handle display
    switch (data->state){
        case LOCKED_UNLOCKING_STATE:
            badge_locked = 0;
            slot_Setup(slot_taskData);
            marquee("UNLOCKING...", data->delayTickCount);
            // add unlocking LEDs
            if (data->delayTickCount % LOCKED_DELAY_BASE == 0){
                locked_leds = locked_lock_base_leds | locked_get_unlock_leds();
            }
            
            // check if unlocked
            if (data->delayTickCount == LOCKED_DELAY_UNLOCKING){
                data->state = LOCKED_UNLOCKED_STATE;
                data->delayTickCount = 0;
            }
            else {
                data->delayTickCount += 1;
            }
            break;
        case LOCKED_LOCKING_STATE:
            marquee("LOCKING...", data->delayTickCount);
            // add locking LEDs
            if (data->delayTickCount % LOCKED_DELAY_BASE == 0){
                locked_leds = locked_lock_base_leds | locked_get_lock_leds();
            }
            
            // check if locked
            if (data->delayTickCount == LOCKED_DELAY_LOCKING){
                data->state = LOCKED_LOCKED_STATE;
                data->delayTickCount = 0;
            }
            else {
                data->delayTickCount += 1;
            }
            break;
        case LOCKED_UNLOCKED_STATE:
            badge_locked = 0;
            // set only base LEDs
            marquee("OPEN", data->delayTickCount);
            locked_unlock_leds();
            break;
        case LOCKED_LOCKED_STATE:
            badge_locked = 1;
            marquee("LOCK", data->delayTickCount);
            // set all LEDs
            locked_lock_leds();
            break;
    }
    nullifyBadge_userLedsSet(data->badge, locked_leds);
    
    // handle pressed buttons
    buttonPressed = nullifyBadge_buttonsGet();
    switch (buttonPressed) {
        case BUTTON_X:
            locked_clear(data);
            currentTask = MENU_TASK;
            break;
        case BUTTON_Y:
            if (data->holdCountY == 0){
                //badge_locked = ~badge_locked;
                pw_found = locked_secret_check(data, BUTTON_Y );
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY += 1;
            data->noholdCount = 0;
            break;
        case BUTTON_B:
            if (data->holdCountB == 0){
                pw_found = locked_secret_check(data, BUTTON_B );
            }
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_A:
            if (data->holdCountA == 0){
                pw_found = locked_secret_check(data, BUTTON_A );
            }
            data->holdCountA += 1;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount = 0;
            break;
        default:
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->noholdCount += 1;
    }
    if(pw_found==1){
                if (badge_locked){
                    // just unlocked
                    data->state = LOCKED_UNLOCKING_STATE;
                }
                else {
                    // just locked
                    data->state = LOCKED_LOCKING_STATE;
                }
    }
}

void locked_clear(struct t_locked_taskData *data){
    /* Initialize application data */
    data->delayTickCount = 0;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->noholdCount = 0;
    
    // clear display and led
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
}


void locked_Exit(void *taskData) {
    struct t_locked_taskData *data = (struct t_locked_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware
    
    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
}

