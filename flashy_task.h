
// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef FLASHY_EXAMPLETASK_H
#define	FLASHY_EXAMPLETASK_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#define FLASHY_LED_DELAY 35
#define FLASHY_LED_DELAY_SHOT 5

#define FLASHY_BLASTER_DELAY 100
#define FLASHY_HIT_DELAY 150

enum t_flashy_led_states {
    FLASHY_STATE_LED_ROUNDABOUT,
    FLASHY_STATE_LED_SHOT,
    FLASHY_STATE_SPARKLE,
    FLASHY_STATE_CIRCLE,
    FLASHY_STATE_LED_ALT,
};

enum t_flashy_display_states {
    FLASHY_STATE_DISPLAY,
    FLASHY_DEFAULT_STATE_DISPLAY,
    FLASHY_STATE_DISPLAY_BLASTER,
    FLASHY_STATE_DISPLAY_SHOT,
    FLASHY_STATE_DISPLAY_OFF,
    FLASHY_STATE_DISPLAY_SECRET_TEST,
};

enum t_flashy_blaster_states {
    FLASHY_BLASTER_SHOOTING_STATE,
    FLASHY_BLASTER_IDLE_STATE,
};


struct t_flashy_taskData {
    /* Badge Hardware */
    struct t_nullifyBadge *badge;
    
    /* Internal App Data */
    u16 delayTickCount;
    enum t_flashy_led_states led_state;
    
    u16 displayTickCount;
    enum t_flashy_display_states display_state;
    
    u16 blasterTickCount;
    enum t_flashy_blaster_states blaster_state;
    
    u16 holdCountA;
    u16 holdCountB;
    u16 holdCountX;
    u16 holdCountY;
    u16 holdCountXY;
    u16 holdCountAY;
    u16 holdCountAX;
    u16 holdCountBX;
    u16 holdCountBY;
    u16 noholdCount;
};

void flashy_Init(void *taskData);
void flashy_Main (void *taskData);
void flashy_Exit(void *taskData);


#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* FLASHY_EXAMPLETASK_H */

