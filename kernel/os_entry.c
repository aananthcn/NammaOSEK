#include <stdio.h>
#include <board.h>
#include <osek.h>

#include <os_api.h>
#include <os_task.h>

#include <sg_appmodes.h>
#include <sg_tasks.h>
#include <os_api.h>

#include <zephyr/kernel.h> /* for k_sleep() */



#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(os_entry, LOG_LEVEL_DBG);



// Macros


// Global varialbes
int OsAppMode;


// Global Functions
int SetActiveApplicationMode(AppModeType mode) {
	if (mode >= OS_MODES_MAX) {
		LOG_ERR("Error: Invalid mode! New mode %d > MAX (%d)", mode, OS_MODES_MAX);
		return -1;
	}

	OsAppMode = mode;
	return 0;
}


AppModeType GetActiveApplicationMode(void) {
	return OsAppMode;
}


void StartOS(AppModeType mode) {
	DisableAllInterrupts();
	SetActiveApplicationMode(mode);
	if (mode != OSDEFAULTAPPMODE) {
		LOG_ERR("%s::%s(): Error: Invalid OS START mode!", __FILE__, __func__);
		LOG_INF("Info: Expected mode == %d mode, actual == %d!", 
			OSDEFAULTAPPMODE, mode);
		return;
	}
	
	OsSetupScheduler(mode);
	EnableAllInterrupts();

	while (OsAppMode == OSDEFAULTAPPMODE) {
		OsScheduleTasks();
		k_sleep(K_TICKS(1));
	}
}
