

#include <xc.h>
#include <stdarg.h>
#include <stdio.h>
#include "xc8_types.h"
#include "nullifyBadge.h"
#include "cooperative_scheduler.h"
#include "matrix_task.h"

// FOLLOW THE WHITE RABBIT NEO
char *matrix_option[] = {
    "  FOLL", // A
    "OW    ", // Y
    " THE WH ", // B
    "ITE   ", // X
    " RAB", // AX
    "  BIT N  ", // AY
    "EO   ", // BA
    "POTAT  ", //bx
    "  EW  ",   //xy
    "  TEY  "        //by
};   
u16 matrix_option_idx;
u16 matrix_option_size;
u16 matrix_leds_idx;

// secret input arrays of buttons, BUTTON_A, BUTTON_B, BUTTON_X, BUTTON_Y,
struct matrix_secret_input 
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
struct matrix_secret_input matrix_secret_inputs[] = {
    {
        // ^^vv<><>ba
        { BUTTON_A, BUTTON_Y, BUTTON_B, BUTTON_X, BUTTON_A| BUTTON_X, BUTTON_A | BUTTON_Y, BUTTON_B | BUTTON_A },

        // set to initial index of 0
        0,

        // test secret, (extern in nullifyBadge.h && set 0 in main.c && used in flashy_task))
        &secret_value,

        // which test to set
        SECRET_VALUE_TEST,
    },
};
 
u8 matrix_found;


void matrix_Init(void *taskData){
    struct t_matrix_taskData *data = (struct t_matrix_taskData *) taskData;

    /* Register Badge Hardware to be Used */
    nullifyBadge_userLedsRegister(data->badge);
    nullifyBadge_userLedsClr(data->badge);
    nullifyBadge_segDisplayRegister(data->badge);
    nullifyBadge_segDisplayErase(data->badge);
    matrix_found =0;

    data->display_state = MATRIX_STATE_DISPLAY;
    data->martix_state = STILL_IN_MATRIX;
    matrix_leds_idx =0;
    
    /* Initialize application data */
    data->holdCountA = 0 ;
    data->holdCountB = 0 ;
    data->holdCountX = 0 ;
    data->holdCountY = 0 ;
    data->holdCountXY = 0;
    data->holdCountAY = 0;
    data->holdCountAX = 0;
    data->holdCountBX = 0;
    data->holdCountBY = 0;
    data->holdCountBA = 0;
    data->noholdCount = 0;   
}

u8 matrix_secret_check(struct t_matrix_taskData *data, u8 buttonPressed){
    u8 *inputs;
    u16 size = sizeof(matrix_secret_inputs) / sizeof(matrix_secret_inputs[0]);
    u16 currents_size;
    char hit_counter[6];
    
    for (u16 i = 0; i < size; i++){
        inputs = matrix_secret_inputs[i].inputs;

        if (buttonPressed == inputs[matrix_secret_inputs[i].idx]){
            currents_size = strlen((const char *)inputs);
            matrix_secret_inputs[i].idx++;

            if (matrix_secret_inputs[i].idx == currents_size) {
                // set index back to 0
                matrix_secret_inputs[i].idx = 0;
                
                // set variable
                *(matrix_secret_inputs[i].variable) = (u16) matrix_secret_inputs[i].value;

                return 1;
            }
        } else {
            // wrong input, set index to 0
            matrix_secret_inputs[i].idx = 0;
        }
    }
    return 0;
}

void matrix_display_option(struct t_matrix_taskData *data){
    
    if(badge_locked ==1){//it's locked
        marquee_speedy("YOU ARE IN TOO DEEP     TAKE THE RED PILL    ", data->noholdCount, 40);
        data->display_state = MATRIX_STATE_DISPLAY;
        data->martix_state = STILL_IN_MATRIX;
        return;
    }
    
    if (data->display_state == MATRIX_DONE){
        marquee_speedy("YOU STAY IN WONDERLAND    ", data->noholdCount, 15);
    }
    else if (data->display_state != MATRIX_STATE_DISPLAY ){
              //  marquee_speedy(matrix_option[matrix_option_idx], data->noholdCount, 10);
                marquee_speedy(matrix_option[data->display_state], data->noholdCount, 20);
        }
    else{
        marquee_speedy("THE CHOICE IS YOURS   ", data->noholdCount, 30);
    }
    
    if (data -> displayTickdown == 0 && data->display_state != MATRIX_DONE ){
        data->display_state = MATRIX_STATE_DISPLAY;
    }
}

void matrix_Main (void *taskData ){
    struct t_matrix_taskData *data = (struct t_matrix_taskData *) taskData ;
    u8 buttonPressed  = 0 ;
    u8 holdvalue = 50;
    
    if ( currentTask != MATRIX_TASK ){
        data->holdCountA = 1;
        data->holdCountB = 1;
        data->holdCountX = 1;
        data->holdCountY = 1;
        return;
    }
    
    
 
   if (data->martix_state == STILL_IN_MATRIX){
        if(data->delayTickCount == 0){
                led_Parody(matrix_leds_idx);
                matrix_leds_idx += 1;
                data->delayTickCount = 10;
            }
            else {
                data->delayTickCount -= 1;
            }
   }
    
    
    
    buttonPressed = nullifyBadge_buttonsGet();
    
          
    switch (buttonPressed) {
        case BUTTON_A:
            // confirm button isn't being held down
            if (data->holdCountA == holdvalue){
                matrix_found = matrix_secret_check(data, buttonPressed);   
                data->display_state = MATRIX_A;
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
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_B:
            // confirm button isn't being held down
            if (data->holdCountB == holdvalue){
                matrix_found = matrix_secret_check(data, buttonPressed); 
                data->display_state = MATRIX_B;
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
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_X:
            // confirm button isn't being held down
            if (data->holdCountX == holdvalue){
                matrix_found = matrix_secret_check(data, buttonPressed); 
                data->display_state = MATRIX_X;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX += 1;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_Y:
            // confirm button isn't being held down
            if (data->holdCountY == holdvalue){
                matrix_found = matrix_secret_check(data, buttonPressed);   
                data->display_state = MATRIX_Y;
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
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_X | BUTTON_Y:
            // confirm button isn't being held down
            if (data->holdCountXY == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_X | BUTTON_Y);
                data->display_state = MATRIX_XY;
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
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_A | BUTTON_Y:
            // confirm button isn't being held down
            if (data->holdCountAY == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_A | BUTTON_Y);
                data->display_state = MATRIX_AY;
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
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_A | BUTTON_X:
            // confirm button isn't being held down
            if (data->holdCountAX == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_A | BUTTON_X);
                data->display_state = MATRIX_AX;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX += 1;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_B | BUTTON_X:
            // confirm button isn't being held down
            if (data->holdCountBX == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_B | BUTTON_X);
                data->display_state = MATRIX_BX;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX += 1;
            data->holdCountBY = 0;
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        case BUTTON_B | BUTTON_Y:
            // confirm button isn't being held down
            if (data->holdCountBY == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_B | BUTTON_Y);
                data->display_state = MATRIX_BY;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY += 1;
            data->holdCountBA = 0;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            //matrix_display_option(data);
            break;
        case BUTTON_B | BUTTON_A:
            // confirm button isn't being held down
            if (data->holdCountBA == holdvalue){
                matrix_found = matrix_secret_check(data, BUTTON_B | BUTTON_A);
                data->display_state = MATRIX_BA;
            }
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->holdCountBA += 1;
            data->noholdCount = 0;
            data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
            break;
        default:
            data->noholdCount += 1;
            data->holdCountA = 0;
            data->holdCountB = 0;
            data->holdCountX = 0;
            data->holdCountY = 0;
            data->holdCountXY = 0;
            data->holdCountAY = 0;
            data->holdCountAX = 0;
            data->holdCountBX = 0;
            data->holdCountBY = 0;
            data->holdCountBA = 0;
            if (matrix_found){
                data->display_state = MATRIX_DONE;
                data->martix_state = MATRIX_SOLVED;
                break;
            }
            data->displayTickdown -= 1;
            matrix_display_option(data);
    }
    if (matrix_found){
            data->display_state = MATRIX_DONE;
            data->martix_state = MATRIX_SOLVED;
        }
    
    if(data->display_state == MATRIX_DONE){
        //matrix_display_option(data);
        if (data->delayTickCount < 1){
                led_curtain(data->noholdCount);
                data->delayTickCount = 20;
            }
            else {
                data->delayTickCount -= 1;
            } 
        /*
                    // send IR once
            if (data->matrix_blasterTickCount == MATRIX_FLASHY_BLASTER_DELAY) {
                nullifyBadge_irTxRegister(data->badge);
                nullifyBadge_irTxSet();
            } 
            if (data->matrix_blasterTickCount == MATRIX_FLASHY_BLASTER_DELAY - 1 ){
                nullifyBadge_irTxClr();
                nullifyBadge_irTxRelease(data->badge);
            }
            else {
                data->matrix_blasterTickCount -= 1;
            } */
    }
}

void matrix_secret_clear(struct t_matrix_taskData *data){
    /* Initialize application data */
    data->displayTickdown = MATRIX_DELAY_TICK_COUNT;
    data->holdCountA = 0;
    data->holdCountB = 0;
    data->holdCountX = 0;
    data->holdCountY = 0;
    data->holdCountXY = 0;
    data->holdCountAY = 0;
    data->holdCountAX = 0;
    data->holdCountBX = 0;
    data->holdCountBY = 0;
    data->holdCountBA = 0;
    data->noholdCount = 0;
}

void matrix_Exit(void *taskData){
    struct t_matrix_taskData *data = (struct t_matrix_taskData *) taskData;
    nullifyBadge_userLedsClr(data->badge); // erase app data
    nullifyBadge_userLedsRelease(data->badge); // un-register hardware

    nullifyBadge_segDisplayErase(data->badge); // erase application data from display
    nullifyBadge_segDisplayRelease(data->badge); // un-register hardware
    
}
