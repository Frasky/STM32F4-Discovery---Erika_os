#include "ee.h"





/***************************************************************************
 *
 * Kernel ( CPU 0 )
 *
 **************************************************************************/
    /* Definition of task's body */
    DeclareTask(BlinkLed_0);
    DeclareTask(BlinkLed_1);
    DeclareTask(BlinkLed_2);
    DeclareTask(TaskIOToggle);

    const EE_THREAD_PTR EE_hal_thread_body[EE_MAX_TASK] = {
        &FuncBlinkLed_0,		/* thread BlinkLed_0 */
        &FuncBlinkLed_1,		/* thread BlinkLed_1 */
        &FuncBlinkLed_2,		/* thread BlinkLed_1 */
        &FuncTaskIOToggle 		/* thread TaskIOToggle */

    };

    /* ready priority */
    const EE_TYPEPRIO EE_th_ready_prio[EE_MAX_TASK] = {
        0x1U,		/* thread BlinkLed_0 */
        0x1U,		/* thread BlinkLed_1 */
        0x1U,		/* thread BlinkLed_2 */
        0x1U 		/* thread TaskIOToggle */
    };

    /* dispatch priority */
    const EE_TYPEPRIO EE_th_dispatch_prio[EE_MAX_TASK] = {
        0x1U,		/* thread BlinkLed_0 */
        0x1U,		/* thread BlinkLed_1 */
        0x1U,		/* thread BlinkLed_2 */
        0x1U 		/* thread TaskIOToggle */
    };

    /* thread status */
    #if defined(__MULTI__) || defined(__WITH_STATUS__)
        EE_TYPESTATUS EE_th_status[EE_MAX_TASK] = {
            EE_READY,
            EE_READY,
            EE_READY,
            EE_READY
        };
    #endif

    /* next thread */
    EE_TID EE_th_next[EE_MAX_TASK] = {
        EE_NIL,
        EE_NIL,
        EE_NIL,
        EE_NIL
    };

    EE_TYPEPRIO EE_th_nact[EE_MAX_TASK];
    /* The first stacked task */
    EE_TID EE_stkfirst = EE_NIL;

    /* The first task into the ready queue */
    EE_TID EE_rqfirst  = EE_NIL;

    /* system ceiling */
    EE_TYPEPRIO EE_sys_ceiling= 0x0000U;



/***************************************************************************
 *
 * Counters
 *
 **************************************************************************/
    EE_counter_RAM_type       EE_counter_RAM[EE_MAX_COUNTER] = {
        {0, -1}         /* myCounter */
    };



/***************************************************************************
 *
 * Alarms
 *
 **************************************************************************/
    const EE_alarm_ROM_type   EE_alarm_ROM[] = {
        {0, EE_ALARM_ACTION_TASK    , BlinkLed_0, NULL},
        {0, EE_ALARM_ACTION_TASK    , BlinkLed_1, NULL},
        {0, EE_ALARM_ACTION_TASK    , BlinkLed_2, NULL}
    };

    EE_alarm_RAM_type         EE_alarm_RAM[EE_MAX_ALARM];

