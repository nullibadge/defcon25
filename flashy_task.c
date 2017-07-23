/*
 * Default task
 */


#include <xc.h>
#include <stdarg.h>
#include <stdio.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "flashy_task.h"





u16 flashy_leds_idx;

u16 flashy_sparkle_rate;
u16 flashy_scroll_rate;

const char *FLASHY_TEXT = "NULL";
const char *FLASHY_TEXT_SECRET_TEST = "\x05\x07\x07\x08--  --\x09\x07\x07\x06 \x0B ";
u16 deaths = 0;


char *flashy_option[] = {
    "NULL",
    "NULLIFY ",
    "DEFCON ",
    "HAX  "
};
u16 flashy_option_idx;
u16 flashy_option_size;


void flashy_Init(void *taskData) {
    struct t_flashy_taskData *data = (struct t_flashy_taskData *) taskData;
    
    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    
    //nullifyBadge_irTxRegister(data->badge);
    //nullifyBadge_irTxClr();
    
    /* Initialize application data */
    data->delayTickCount = FLASHY_LED_DELAY;
    data->displayTickCount = 0;
    
    /* Initialize button Presses */
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->holdCountXY = 0;
    data->holdCountAY = 0;
    data->holdCountAX = 0;
    data->noholdCount = 0;
    
    flashy_leds_idx = 0;
    data->led_state = FLASHY_STATE_LED_ROUNDABOUT;
    flashy_option_idx = 0;
    
    flashy_option_size = sizeof(flashy_option)/sizeof(flashy_option[0]);
    
    flashy_sparkle_rate =50;
    flashy_scroll_rate=50;
    
}


void flashy_display_option(struct t_flashy_taskData *data){
    if (flashy_option_idx == 0){
        nullifyBadge_segDisplayPutStr(data->badge, flashy_option[flashy_option_idx]);
        nullifyBadge_segDisplayForceUpdate(data->badge);
    }
    else {
        if (data->display_state==FLASHY_STATE_DISPLAY ){
                  //  marquee_speedy(flashy_option[flashy_option_idx], data->noholdCount, 10);
                    marquee_speedy(flashy_option[flashy_option_idx], data->noholdCount, flashy_scroll_rate);
        }
        else{
            marquee(flashy_option[flashy_option_idx], data->noholdCount);
        }
    }
}


void flashy_Main (void *taskData) {
    struct t_flashy_taskData *data = (struct t_flashy_taskData *) taskData;
    u16 buttonPressed;
    char hit_counter[6];
    
    if ( currentTask != FLASHY_TASK ){
        data->holdCountA = 1;
        data->holdCountB = 1;
        data->holdCountX = 1;
        data->holdCountY = 1;
        data->holdCountXY = 1;
        data->holdCountAY = 1;
        data->holdCountAX = 1;
        return;
    }
    
    // test secret
    switch (secret_value){
        case SECRET_VALUE_INIT:
            break;
        case SECRET_VALUE_TEST:
            secret_value = SECRET_VALUE_INIT;
            data->displayTickCount = 0;
            data->display_state = FLASHY_STATE_DISPLAY_SECRET_TEST;
            
            data->holdCountA = 1;
            data->holdCountB = 1;
            data->holdCountX = 1;
            data->holdCountY = 1;
            data->holdCountXY = 1;
            data->holdCountAY = 1;
            data->holdCountAX = 1;
            data->noholdCount = 1;
            return;
            break;
    }

    // handle LEDs
    switch (data->led_state){
        case FLASHY_STATE_LED_ROUNDABOUT:
            if (data->delayTickCount == 0){
                led_roundabout(flashy_leds_idx);
                flashy_leds_idx += 1;
                data->delayTickCount = FLASHY_LED_DELAY;
            }
            else {
                data->delayTickCount -= 1;
            }
            break;
        case FLASHY_STATE_LED_SHOT:
            if (data->delayTickCount == 0){
                led_curtain(flashy_leds_idx);
                flashy_leds_idx += 1;
                data->delayTickCount = FLASHY_LED_DELAY_SHOT;
            }
            else {
                data->delayTickCount -= 1;
            }
            break;
        case FLASHY_STATE_SPARKLE:
            if (data->delayTickCount < 1){
                led_rand();
                data->delayTickCount = flashy_sparkle_rate;
            }
            else {
                data->delayTickCount -= 1;
            }
        case FLASHY_STATE_CIRCLE:
            if (data->delayTickCount ==0 ){
                if (flashy_leds_idx == 1024 ){
                    flashy_leds_idx = flashy_leds_idx << 1;
                }
                led_circle(flashy_leds_idx);
                if (flashy_leds_idx < 1 ){
                    flashy_leds_idx = 1;
                    break;
                }
                flashy_leds_idx = flashy_leds_idx << 1;

                data->delayTickCount = flashy_sparkle_rate;
            }
            else {
                data->delayTickCount -= 1;
            }
    }
    
    // handle display
    switch (data->display_state){
        case FLASHY_STATE_DISPLAY_BLASTER:
            nullifyBadge_segDisplayPutStr(data->badge, "PEW ");
            nullifyBadge_segDisplayForceUpdate(data->badge);
            data->displayTickCount += 1;
            break;
        case FLASHY_STATE_DISPLAY_SHOT:
            sprintf((char *)hit_counter, "%4d", deaths);
            
            nullifyBadge_segDisplayPutStr(data->badge, hit_counter);
            nullifyBadge_segDisplayForceUpdate(data->badge);
            if (data->displayTickCount == FLASHY_HIT_DELAY){
                data->displayTickCount = 0;
                data->display_state = FLASHY_LED_DELAY;
            }
            data->displayTickCount += 1;
            break;
        case FLASHY_STATE_DISPLAY_SECRET_TEST:
            marquee(FLASHY_TEXT_SECRET_TEST, data->displayTickCount);
            data->displayTickCount += 1;
            break;
        case FLASHY_STATE_DISPLAY:
            //break;
        default:
            flashy_display_option(data);
            data->displayTickCount += 1;
            break;
    }
    
    // handle button input
    buttonPressed = nullifyBadge_buttonsGet();
    switch (buttonPressed) {
        case BUTTON_X | BUTTON_Y:
            if (data->holdCountXY == 0 ){
                // set back to default display
                data->display_state = FLASHY_STATE_DISPLAY;
            }
            
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY += 1;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
         case BUTTON_A | BUTTON_Y:
            if ( data->holdCountAY == 0 ){
                if (flashy_sparkle_rate > 1000){
                    flashy_sparkle_rate = 1000;
                }
                flashy_sparkle_rate += 5;
            }
            
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY += 1;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;            
            data->noholdCount = 0;
            break;
        case BUTTON_A | BUTTON_X:
            if ( data->holdCountAX == 0 ){
                if (flashy_sparkle_rate < 5)
                    flashy_sparkle_rate=5;
                flashy_sparkle_rate -= 5;
            }

            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX += 1;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX += 1;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_B | BUTTON_Y: 
            if (data->holdCountBY == 0){
                if (flashy_option_idx == 0){ //fade down
                    
                }
                else { // slow down
                    if (flashy_scroll_rate > 150){
                        flashy_scroll_rate =150;
                    }
                    flashy_scroll_rate +=2;
                }

            }
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX = 0;
            data->holdCountY += 1;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY += 1;
            data->noholdCount = 0;
            break;
        case BUTTON_B | BUTTON_X: 
            if (data-> holdCountBX == 0){
                if (flashy_option_idx == 0){ //Fade up

                }
                else { //speed up
                    if (flashy_scroll_rate < 5){
                        flashy_scroll_rate =5;
                    }
                    flashy_scroll_rate -=3;
                }
                
            }
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX += 1;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX += 1;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_Y:
            // start shooting
            if (data->holdCountY == 0 && data->blasterTickCount == 0){
                // turn off LEDs
                nullifyBadge_userLedsSet(data->badge, 0x0);

                data->displayTickCount = 0;
                data->blasterTickCount = FLASHY_BLASTER_DELAY;
                data->delayTickCount = FLASHY_LED_DELAY_SHOT;

                data->led_state = FLASHY_STATE_LED_SHOT;
                data->display_state = FLASHY_STATE_DISPLAY_BLASTER;
                data->blaster_state = FLASHY_BLASTER_SHOOTING_STATE;
            }

            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY += 1;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_B:
            // scroll one way
            if (data->holdCountB == 0){
                data->display_state = FLASHY_STATE_DISPLAY;
                if (flashy_option_idx == flashy_option_size - 1){
                        flashy_option_idx = 0;
                    }
                else {
                        flashy_option_idx += 1;
                    }
            }
            data->holdCountA = 0;
            data->holdCountB += 1;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
        case BUTTON_A:
            if ( data->holdCountA == 0){
                if (data->led_state != FLASHY_STATE_SPARKLE){
                    data->led_state = FLASHY_STATE_SPARKLE;
                }
                else {
                    data->led_state = FLASHY_STATE_CIRCLE;
                }
            }
            
            data->holdCountA += 1;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
            break;
        case NO_BUTTON:

            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount += 1;
            break;
        default:
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->noholdCount = 0;
    }
    
    // handle shooting
    switch (data->blaster_state) {
        case FLASHY_BLASTER_SHOOTING_STATE:
            // send IR once
            if (data->blasterTickCount == FLASHY_BLASTER_DELAY) {
                nullifyBadge_irTxRegister(data->badge);
                nullifyBadge_irTxSet();
            } 
            if (data->blasterTickCount == FLASHY_BLASTER_DELAY - 1 ){
                nullifyBadge_irTxClr();
                nullifyBadge_irTxRelease(data->badge);
            }
            if (data->blasterTickCount == 0){
                data->displayTickCount = 0;
                data->delayTickCount = FLASHY_LED_DELAY_SHOT;

                data->led_state = FLASHY_STATE_LED_ROUNDABOUT;
                data->display_state = FLASHY_STATE_DISPLAY;
                data->blaster_state = FLASHY_BLASTER_IDLE_STATE;
            }
            else {
                data->blasterTickCount -= 1;
            }
            break;
        case FLASHY_BLASTER_IDLE_STATE:
            break;
    }

    // handle shot
    if (nullifyBadge_irGet() && (data->blaster_state != FLASHY_BLASTER_SHOOTING_STATE)) {
        data->displayTickCount = 0;
        deaths += 1;
        data->display_state = FLASHY_STATE_DISPLAY_SHOT;
    }
}

void flashy_Exit(void *taskData) {
    struct t_flashy_taskData *data = (struct t_flashy_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware
    
    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
}

