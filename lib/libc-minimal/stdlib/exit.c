/*
 * Copyright (c) 2019 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdio.h>

void _exit(int status)
{
	printf("exit\n");
	while (1) {
	}
}
