/*
 * Copyright (c) 2020 Linaro Limited
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdlib.h>
#include <stdio.h>

void _abort(void)
{
	printf("abort() - implement abort handler!\n");
}
