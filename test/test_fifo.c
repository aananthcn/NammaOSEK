#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#include <os_fifo.h>
#include <osek.h>


typedef enum {
	OS_QUEUE_WAITING,
	OS_QUEUE_SUSPENDED,
	OS_QUEUE_READY,
	OS_QUEUE_RUNNING,
        OS_QUEUE_MAX
}OsQueueType;


#define OS_FIFO_SIZE (128)

#define OS_FIFO(x)              (x##_fifo)
#define OS_TASK_BUF(x, y)       (x##_buffer[y])
#define DefineFifoQueue(name, len) \
		OsTaskType* OS_TASK_BUF(name, len); \
		OsFifoType OS_FIFO(name) = { \
			.task = (OsTaskType**) &name##_buffer, \
			.head = 0, \
			.tail = 0, \
			.size = (u32) len, \
			.full = false \
		};

#endif



extern OsFifoType* TaskQueues[];

const OsTaskType OsTaskList[10];


DefineFifoQueue(OS_QUEUE_WAITING, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_SUSPENDED, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_READY, OS_FIFO_SIZE);
DefineFifoQueue(OS_QUEUE_RUNNING, OS_FIFO_SIZE);


OsFifoType* TaskQueues[] = {
	&OS_FIFO(OS_QUEUE_WAITING),
	&OS_FIFO(OS_QUEUE_SUSPENDED),
	&OS_FIFO(OS_QUEUE_READY),
	&OS_FIFO(OS_QUEUE_RUNNING)
};





void print(const char *str, ...)
{
	int len = strlen(str);
	int i;

	va_list arg;
	int done;

	va_start(arg, str);
	done = vfprintf(stdout, str, arg);
	va_end(arg);

	printf("\n");
	for(i = 0; i < len; i++, printf("%c", '-'));
	printf("\n");
}

int main(void)
{
	int i, len;
	char str[1024];

	print("Expect %d prints with size %d each!", OS_QUEUE_MAX, OS_FIFO_SIZE);
	for (i = 0; i < OS_QUEUE_MAX; i++) {
		printf("size[%d] = %d\n", i, TaskQueues[i]->size);
	}
	
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoRead(OS_QUEUE_WAITING);
	OsFifoRead(OS_QUEUE_WAITING);
	OsFifoRead(OS_QUEUE_WAITING);
	OsFifoRead(OS_QUEUE_WAITING);
	
	print("\n\nExpecting 2 x 2 FIFO full errors");
	for (i = 0; i < OS_FIFO_SIZE+2; i++) {
		OsFifoWrite(OS_QUEUE_WAITING, 2);
		OsFifoWrite(OS_QUEUE_RUNNING, 1);
	}

	print("\n\nExpecting 2 FIFO empty errors");
	for (i = 0; i < OS_FIFO_SIZE+2; i++) {
		OsFifoRead(OS_QUEUE_WAITING);
	}

	print("\n\nExpecting no errors here");
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoRead(OS_QUEUE_WAITING);
	OsFifoWrite(OS_QUEUE_WAITING, 2);
	OsFifoRead(OS_QUEUE_WAITING);

	print("testing is complete! Cleaning-up the directory!");
}