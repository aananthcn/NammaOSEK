#ifndef ARM_INCLUDE_MACROS_H
#define ARM_INCLUDE_MACROS_H

/*
 * Author: Aananth C N
 * Date: 27 Jun 2021
 * This file is taken from zephyr project (Apache 2.0)
 */

.macro section_var section, symbol
	.section .\section\().\symbol
	\symbol :
.endm

.macro section_func section, symbol
	.section .\section\().\symbol, "ax"
	FUNC_CODE()
	PERFOPT_ALIGN
	\symbol :
	FUNC_INSTR(\symbol)
.endm

.macro section_subsec_func section, subsection, symbol
	.section .\section\().\subsection, "ax"
	PERFOPT_ALIGN
	\symbol :
.endm

#define SECTION_VAR(sect, sym) section_var sect, sym
#define SECTION_FUNC(sect, sym) section_func sect, sym
#define SECTION_SUBSEC_FUNC(sect, subsec, sym) \
	section_subsec_func sect, subsec, sym


#endif