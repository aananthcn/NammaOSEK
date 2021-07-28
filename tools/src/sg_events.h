#ifndef ACN_OSEK_SG_EVENTS_H
#define ACN_OSEK_SG_EVENTS_H

#include <osek.h>


/*  Event Masks for Task_A  */

/*  Event Masks for Task_B  */
#define EVENT_MASK_Task_B_event1	(0x0000000000000001)

/*  Event Masks for Task_C  */
#define EVENT_MASK_Task_C_event1	(0x0000000000000001)
#define EVENT_MASK_Task_C_event2	(0x0000000000000002)

/*  Event array for Task_A  */

/*  Event array for Task_B  */
extern const EventMaskType Task_B_EventMasks[];

/*  Event array for Task_C  */
extern const EventMaskType Task_C_EventMasks[];


#endif
