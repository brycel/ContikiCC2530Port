/*
 * Copyright (c) 2011, George Oikonomou - <oikonomou@users.sourceforge.net>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the Institute nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE INSTITUTE AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * This file is part of the Contiki operating system.
 */

/**
 * \file
 *   Definitions for TI/Chipcon cc2530, cc2531 and cc2533 SFR registers.
 *
 *   Based on information in:
 *   "CC253x System-on-Chip Solution for 2.4-GHz IEEE 802.15.4 and ZigBee®
 *    Applications"
 *   Literature Number: SWRU191B. April 2009–Revised September 2010
 *
 * \author
 *         George Oikonomou - <oikonomou@users.sourceforge.net>
 */

#ifndef __CC253X_H__
#define __CC253X_H__

/*---------------------------------------------------------------------------*/
/* Compiler Abstraction */
#if (chip == 2531)
  #include "ioCC2531.h"
#else
  #include "ioCC2530.h"
#endif

#define X_IEEE_ADDR (*(uint8_t*)(P_INFOPAGE + 0x0C))
#define OVFIM T1OVFIM

#endif /* __CC253X_H__ */
