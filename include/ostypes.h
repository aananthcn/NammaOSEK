#ifndef OSEK_OS_TYPES_H
#define OSEK_OS_TYPES_H

#include <stdint.h>

// boolean definitions by Aananth
typedef unsigned char 	_bool;
#define bool		_bool
#define true		1
#define false		0

// integers easy to use
typedef uint8_t		u8;
typedef uint16_t	u16;
typedef	uint32_t	u32;
typedef uint64_t	u64;
typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;
typedef int64_t		s64;

#endif