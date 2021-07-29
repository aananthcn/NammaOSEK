#include <stdio.h>
#include <osek.h>


TASK(Task_A) {
	printf("%s\n", __func__);
}

TASK(Task_B) {
	printf("%s\n", __func__);
}

TASK(Task_C) {
	printf("%s\n", __func__);
}