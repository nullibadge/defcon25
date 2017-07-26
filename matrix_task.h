/* 
 * File:   matrix_task.h
 * Author: Austin
 *
 * Created on July 25, 2017, 1:01 PM
 */

#ifndef MATRIX_TASK_H
#define	MATRIX_TASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MATRIX_FLASHY_BLASTER_DELAY 300


#define MATRIX_DELAY_TICK_COUNT 300

enum t_matrix_display_states {
    MATRIX_A,
    MATRIX_Y,
    MATRIX_B,
    MATRIX_X,
    MATRIX_AX,
    MATRIX_AY,
    MATRIX_BA, //FIN
    MATRIX_BX,
    MATRIX_XY,
    MATRIX_BY,
    MATRIX_STATE_DISPLAY,
    MATRIX_DONE
};

enum t_matrix_martix_state{
    MATRIX_SOLVED,
    STILL_IN_MATRIX,
};

struct t_matrix_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    enum t_matrix_martix_state martix_state;

    enum t_matrix_display_states display_state;

    /* Internal App Data */
    u16 delayTickCount;
    u16 matrix_blasterTickCount;
    u16 displayTickdown;
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 holdCountXY;
    u16 holdCountAY;
    u16 holdCountAX;
    u16 holdCountBX;
    u16 holdCountBY;
    u16 holdCountBA;
    u16 noholdCount;
    
};

void matrix_Init(void *taskData);
void matrix_Main (void *taskData);
void matrix_Exit(void *taskData);
void matrix_secret_clear(struct t_matrix_taskData *data);


#ifdef	__cplusplus
extern "C" {
#endif




#ifdef	__cplusplus
}
#endif

#endif	/* MATRIX_TASK_H */

