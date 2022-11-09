#ifndef ACN_OSEK_OS_FIFO_H
#define ACN_OSEK_OS_FIFO_H

#include <base_types.h>
#include <sg_tasks.h>

typedef struct {
	OsTaskType** task;
	u32 head;
	u32 tail;
	const u32 size;
#ifdef DEBUG
	char *name;
#endif
	bool full;
} OsFifoType;


int OsFifoWrite(OsFifoType* pFifoQ, TaskType TaskID);
OsTaskType* OsFifoRead(OsFifoType* pFifoQ);

int AddTaskToFifoQueue(const OsTaskType task, const OsFifoType* fq[]);
OsTaskType* GetTaskFromFifoQueue(const OsFifoType* fq[], u32 prio);


#endif