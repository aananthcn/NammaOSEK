#include <stdio.h>
#include <board.h>
#include <osek.h>


int OsAppMode;

int SetActiveApplicationMode(AppModeType mode) {
	if (mode >= OS_MODES_MAX) {
		printf("Error: Invalid mode! New mode %d > MAX (%d)\n", mode, OS_MODES_MAX);
		return -1;
	}

	OsAppMode = mode;
	return 0;
}

AppModeType GetActiveApplicationMode(void) {
	return OsAppMode;
}

void SetupScheduler() {
	printf("%s:%s() under construction\n", __FILE__, __func__);
}

void RunBackgroundTasks(void) {
	// Add all background tasks such as
	// 1) stack overflow check
	// 2) flash corruption check
	// 3) ram corruption check (running 1's?)
	// 4) board specific background checks
	BoardSpecific_BackgroundTask();
}

void StartOS(AppModeType mode) {
	SetActiveApplicationMode(mode);
	if (mode != OSDEFAULTAPPMODE) {
		printf("%s::%s(): Error: Invalid OS START mode!\n", __FILE__, __func__);
		printf("Info: Expected mode == %d mode, actual == %d!\n", 
			OSDEFAULTAPPMODE, mode);
		return;
	}
	
	SetupScheduler();

	while (OsAppMode == OSDEFAULTAPPMODE) {
		RunBackgroundTasks();
	}
}