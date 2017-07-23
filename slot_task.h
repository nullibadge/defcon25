

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SLOT_EXAMPLETASK_H
#define	SLOT_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define SLOT_DELAY_TICK_COUNT 50


struct t_slot_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    u8 locked;
    
    /* Internal App Data */
    u16 displayTickCount;
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 noholdCount;
};

void slot_Init(void *taskData);
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

