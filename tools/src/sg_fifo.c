#include <stddef.h>
#include "sg_fifo.h"

/* Allocate Buffers in RAM */
OsTaskType* WaitingTasks_1[3];
OsTaskType* SuspendedTasks_1[3];
OsTaskType* ReadyTasks_1[3];
OsTaskType* WaitingTasks_4[1];
OsTaskType* SuspendedTasks_4[1];
OsTaskType* ReadyTasks_4[1];
OsTaskType* RunningTasks[1];


/* Allocate FIFO queues in RAM */
OsFifoType WaitingFifo_1 = {
	.task = WaitingTasks_1,
	.size = 3,
	.head = 0,
	.tail = 0,
	.full = false
};

OsFifoType SuspendedFifo_1 = {
	.task = SuspendedTasks_1,
	.size = 3,
	.head = 0,
	.tail = 0,
	.full = false
};

OsFifoType ReadyFifo_1 = {
	.task = ReadyTasks_1,
	.size = 3,
	.head = 0,
	.tail = 0,
	.full = false
};


OsFifoType WaitingFifo_4 = {
	.task = WaitingTasks_4,
	.size = 1,
	.head = 0,
	.tail = 0,
	.full = false
};

OsFifoType SuspendedFifo_4 = {
	.task = SuspendedTasks_4,
	.size = 1,
	.head = 0,
	.tail = 0,
	.full = false
};

OsFifoType ReadyFifo_4 = {
	.task = ReadyTasks_4,
	.size = 1,
	.head = 0,
	.tail = 0,
	.full = false
};


OsFifoType RunningFifo = {
	.task = RunningTasks,
	.size = 1,
	.head = 0,
	.tail = 0,
	.full = false
};


/* Prioritized OSEK FIFO queues in Flash */
const OsFifoType* WaitingQueue[] = {
	NULL,
	&WaitingFifo_1,
	NULL,
	NULL,
	&WaitingFifo_4
};

const OsFifoType* SuspendedQueue[] = {
	NULL,
	&SuspendedFifo_1,
	NULL,
	NULL,
	&SuspendedFifo_4
};

const OsFifoType* ReadyQueue[] = {
	NULL,
	&ReadyFifo_1,
	NULL,
	NULL,
	&ReadyFifo_4
};

const OsFifoType* RunningQueue[] = {
	&RunningFifo
};

