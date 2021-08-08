#ifndef ACN_OSEK_SG_FIFO_H
#define ACN_OSEK_SG_FIFO_H

#include <os_fifo.h>


#define SG_FIFO_QUEUE_MAX_LEN   (5)

extern const OsFifoType* WaitingQueue[];
extern const OsFifoType* SuspendedQueue[];
extern const OsFifoType* ReadyQueue[];
extern const OsFifoType* RunningQueue[];


#endif