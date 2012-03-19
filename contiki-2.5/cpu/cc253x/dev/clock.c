/*
 * Copyright (c) 2009, Swedish Institute of Computer Science.
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
 *
 * $Id: clock.c,v 1.1 2009/09/08 20:07:35 zdshelby Exp $
 */

/**
 * \file
 *         Implementation of the clock functions for the 8051 CPU
 * \author
 *         Zach Shelby (zach@sensinode.com) - original
 *         George Oikonomou - <oikonomou@users.sourceforge.net> - cc2530 port
 */
#include "sfr-bits.h"
#include "sys/clock.h"
#include "sys/etimer.h"
#include "cc253x.h"
#include "sys/energest.h"

/* Sleep timer runs on the 32k RC osc. */
/* One clock tick is 7.8 ms */
#define TICK_VAL (32768/128)  /* 256 */

#define MAX_TICKS (~((clock_time_t)0) / 2)
/*---------------------------------------------------------------------------*/
/* Do NOT remove the absolute address and do NOT remove the initialiser here */
//__xdata __at(0x0000) unsigned long timer_value = 0; // TODO
static volatile unsigned long timer_value = 0;

static volatile __data clock_time_t count = 0; /* Uptime in ticks */
static volatile __data clock_time_t seconds = 0; /* Uptime in secs */
/*---------------------------------------------------------------------------*/
/**
 * One delay is about 0.6 us, so this function delays for len * 0.6 us
 */
void
clock_delay(unsigned int len)
{
  unsigned int i;
  for(i = 0; i< len; i++) {
    ASM(nop);
  }
}
/*---------------------------------------------------------------------------*/
/**
 * Wait for a multiple of ~8 ms (a tick)
 */
void
clock_wait(int i)
{
  clock_time_t start;

  start = clock_time();
  while(clock_time() - start < (clock_time_t)i);
}
/*---------------------------------------------------------------------------*/
CCIF clock_time_t
clock_time(void)
{
  return count;
}
/*---------------------------------------------------------------------------*/
CCIF unsigned long
clock_seconds(void)
{
  return seconds;
}
/*---------------------------------------------------------------------------*/
/*
 * There is some ambiguity between TI cc2530 software examples and information
 * in the datasheet.
 *
 * TI examples appear to be writing to SLEEPCMD, initialising hardware in a
 * fashion semi-similar to cc2430
 *
 * However, the datasheet claims that those bits in SLEEPCMD are reserved
 *
 * The code here goes by the datasheet (ignore TI examples) and seems to work.
 */
void
clock_init(void)
{
  /* Make sure we know where we stand */
  CLKCONCMD = CLKCONCMD_OSC32K | CLKCONCMD_OSC;

  /* Stay with 32 KHz RC OSC, Chance System Clock to 32 MHz */
  CLKCONCMD &= ~CLKCONCMD_OSC;
  while(CLKCONSTA & CLKCONCMD_OSC);

  /* Tickspeed 500 kHz for timers[1-4] */
  CLKCONCMD |= CLKCONCMD_TICKSPD2 | CLKCONCMD_TICKSPD1;
  while(CLKCONSTA != CLKCONCMD);

  /*Initialize tick value*/
  timer_value = ST0;
  timer_value += ((unsigned long int) ST1) << 8;
  timer_value += ((unsigned long int) ST2) << 16;
  timer_value += TICK_VAL;
  ST2 = (unsigned char) (timer_value >> 16);
  ST1 = (unsigned char) (timer_value >> 8);
  ST0 = (unsigned char) timer_value;
  
  STIE = 1;		/* IEN0.STIE interrupt enable */
}
/*---------------------------------------------------------------------------*/
#ifdef SDCC
  void clock_isr(void) __interrupt(ST_VECTOR)
#else
  #pragma vector=ST_VECTOR
  __near_func __interrupt void clock_isr(void)
#endif
{
  DISABLE_INTERRUPTS();
  ENERGEST_ON(ENERGEST_TYPE_IRQ);

  /*
   * If the Sleep timer throws an interrupt while we are powering down to
   * PM1, we need to abort the power down. Clear SLEEP.MODE, this will signal
   * main() to abort the PM1 transition
   *
   * On cc2430 this would be:
   * SLEEPCMD &= 0xFC;
   */

  /*
   * Read value of the ST0:ST1:ST2, add TICK_VAL and write it back.
   * Next interrupt occurs after the current time + TICK_VAL
   */
  timer_value = ST0;
  timer_value += ((unsigned long int) ST1) << 8;
  timer_value += ((unsigned long int) ST2) << 16;
  timer_value += TICK_VAL;
  ST2 = (unsigned char) (timer_value >> 16);
  ST1 = (unsigned char) (timer_value >> 8);
  ST0 = (unsigned char) timer_value;
  
  ++count;
  
  /* Make sure the CLOCK_CONF_SECOND is a power of two, to ensure
     that the modulo operation below becomes a logical and and not
     an expensive divide. Algorithm from Wikipedia:
     http://en.wikipedia.org/wiki/Power_of_two */
#if (CLOCK_CONF_SECOND & (CLOCK_CONF_SECOND - 1)) != 0
#error CLOCK_CONF_SECOND must be a power of two (i.e., 1, 2, 4, 8, 16, 32, 64, ...).
#error Change CLOCK_CONF_SECOND in contiki-conf.h.
#endif
  if(count % CLOCK_CONF_SECOND == 0) {
    ++seconds;
  }
  
  if(etimer_pending()
      && (etimer_next_expiration_time() - count - 1) > MAX_TICKS) {
    etimer_request_poll();
  }
  
  STIF = 0; /* IRCON.STIF */
  ENERGEST_OFF(ENERGEST_TYPE_IRQ);
  ENABLE_INTERRUPTS();
}
/*---------------------------------------------------------------------------*/
