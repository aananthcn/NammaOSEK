#include <stddef.h>
#include <osek.h>
#include "sg_messages.h"


/*  Messages described in OIL file */
MessageType inter_task_message;


/*  Messages lists for Tasks */
MessageType* Task_A_Messages[] = {
	&inter_task_message
};

MessageType* Task_B_Messages[] = {
	&inter_task_message
};

