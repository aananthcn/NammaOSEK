#include <stddef.h>
#include "sg_resources.h"


/*  Resources described in OIL file */
ResourceType mutex1;


/*  Resources lists for Tasks */
ResourceType* Task_B_Resources[] = {
	&mutex1
};

