
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef LOCKED_EXAMPLETASK_H
#define	LOCKED_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation


#define LOCKED_DELAY_BASE 75

// LOCKED_DELAY_BASE * 5
#define LOCKED_DELAY_LOCKING 375
#define LOCKED_DELAY_UNLOCKING 375

enum t_locked_states {
    LOCKED_LOCKED_STATE,
    LOCKED_LOCKING_STATE,
    LOCKED_UNLOCKED_STATE,
    LOCKED_UNLOCKING_STATE,
};

struct t_locked_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    /* Internal App Data */
    u16 delayTickCount;
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 noholdCount;
    
    enum t_locked_states state;
};

void locked_Init(void *taskData);
void locked_Main (void *taskData);
void locked_Exit(void *taskData);

void locked_clear(struct t_locked_taskData *data);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* LOCKED_EXAMPLETASK_H */

