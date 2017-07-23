

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MENU_EXAMPLETASK_H
#define	MENU_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

#define MENU_DELAY_TICK_COUNT 50


struct t_menu_taskData {
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

void menu_Init(void *taskData);
void menu_Main (void *taskData);
void menu_Exit(void *taskData);

void menu_display_option(struct t_menu_taskData *data);
void menu_run_option(struct t_menu_taskData *data);
void menu_clear(struct t_menu_taskData *data);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MENU_EXAMPLETASK_H */

