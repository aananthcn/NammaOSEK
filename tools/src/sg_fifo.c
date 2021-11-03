#include <stddef.h>
#include "sg_fifo.h"

/* Allocate Buffers in RAM */
OsTaskType* ReadyTasks_1[3];
OsTaskType* ReadyTasks_4[1];
OsTaskType* RunningTasks[1];


/* Allocate FIFO queues in RAM */
OsFifoType ReadyFifo_1 = {
	.task = ReadyTasks_1,
	.size = 3,
	.head = 0,
	.tail = 0,
#ifdef DEBUG
	.name = "ReadyFifo_1",
#endif
	.full = false
};


OsFifoType ReadyFifo_4 = {
	.task = ReadyTasks_4,
	.size = 1,
	.head = 0,
	.tail = 0,
#ifdef DEBUG
	.name = "ReadyFifo_4",
#endif
	.full = false
};



/* Prioritized OSEK FIFO queues in Flash */
const OsFifoType* ReadyQueue[] = {
	NULL,
	&ReadyFifo_1,
	NULL,
	NULL,
	&ReadyFifo_4
};

