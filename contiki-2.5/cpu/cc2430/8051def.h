/*
 * \file
 *	This file contains a set of configuration for using SDCC as a compiler.
 *	Modified from z80 port for cc2430 port.
 *
 * \author
 *	 Takahide Matsutsuka <markn@markn.org> (Original)
 *	 George Oikonomou - <oikonomou@users.sourceforge.net>
 *	   (recent updates for the sensinode/cc2430 port)
 */

#ifndef __8051_DEF_H__
#define __8051_DEF_H__

#include <stdint.h>

#define CC_CONF_FUNCTION_POINTER_ARGS	1
#define CC_CONF_FASTCALL
#define CC_CONF_VA_ARGS		1
#define CC_CONF_UNSIGNED_CHAR_BUGS	0
#define CC_CONF_REGISTER_ARGS		0
#define CC_CONF_FUNCTION_POINTER_KEYWORD __reentrant

/* Generic types. */
typedef unsigned char   u8_t;      /* 8 bit type */
typedef unsigned short u16_t;      /* 16 bit type */
typedef unsigned long  u32_t;      /* 32 bit type */
typedef   signed long  s32_t;      /* 32 bit type */
typedef unsigned short uip_stats_t;


/* Time type. */
typedef unsigned short clock_time_t;
#define MAX_TICKS (~((clock_time_t)0) / 2)

/* Compiler configurations */
#define CCIF
#define CLIF

/* Critical section management */
#define DISABLE_INTERRUPTS()  do {EA = 0;} while(0)
#define ENABLE_INTERRUPTS()   do {EA = 1;} while(0)

#define ENTER_CRITICAL()	\
{	\
	__asm		\
	push	ACC	\
	push	IE	\
	__endasm;	\
}	\
	EA = 0;

#define EXIT_CRITICAL()	\
{	\
	__asm			\
	pop		ACC	\
	__endasm;		\
	ACC &= 0x80;		\
	IE |= ACC;		\
	__asm			\
	pop		ACC	\
	__endasm; 		\
}

/* Macro for a soft reset. In many respects better than H/W reboot via W/D */
#define SOFT_RESET() {((void (__code *) (void)) 0x0000) ();}

/* We don't provide architecture-specific checksum calculations */
#define UIP_ARCH_ADD32		0
#define UIP_ARCH_CHKSUM	0

#define CC_CONF_ASSIGN_AGGREGATE(dest, src)	\
    memcpy(dest, src, sizeof(*dest))

#define uip_ipaddr_copy(dest, src)		\
    memcpy(dest, src, sizeof(*dest))

#endif /* __8051_DEF_H__ */
