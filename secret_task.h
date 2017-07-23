

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef SECRET_EXAMPLETASK_H
#define	SECRET_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define SECRET_DELAY_TICK_COUNT 300


struct t_secret_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    u8 locked;
    
    /* Internal App Data */
    u16 delayTickCount;
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 noholdCount;
};

void secret_Init(void *taskData);
void secret_Main (void *taskData);
void secret_Exit(void *taskData);

void secret_clear(struct t_secret_taskData *data);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* SECRET_EXAMPLETASK_H */

