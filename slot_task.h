

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SLOT_EXAMPLETASK_H
#define	SLOT_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define SLOT_DELAY_TICK_COUNT 50
#define WHEEL0_CYCLES 100
#define WHEEL1_CYCLES 200
#define WHEEL2_CYCLES 300
#define WHEEL3_CYCLES 400
#define FINAL_CYCLE   500
#define PRINT_BUFF_SIZE 20

/* Global slot wheel array */ 
u8 Slot_Wheel[];

/* Shot options */
u8 *Shot_Options[];

struct t_slot_taskData* slot_taskData;

struct t_slot_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    u8 locked;
    
    /* Internal App Data */
    u16 displayTickCount;
    
    u16 delayTickCount;
    
    /* set wheels index as globals */
    u8 slot_wheel_index[4];
    
    /* print buffer */
    u8 slot_print_buf[PRINT_BUFF_SIZE];
    
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 noholdCount;
};



void slot_Init(void *taskData);
void slot_Setup(void *taskData);
void slot_Running_Animation(void *taskData);
void slot_Finished_Animation(void *taskData);
void slot_Main (void *taskData);
void slot_Exit(void *taskData);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SLOT_EXAMPLETASK_H */

