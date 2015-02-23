/*
 * Copyright (C) 2015 AggieAir, A Remote Sensing Unmanned Aerial System for Scientific Applications
 * Utah State University, http://aggieair.usu.edu/
 *
 * Michal Podhradsky (michal.podhradsky@aggiemail.usu.edu)
 * Calvin Coopmans (c.r.coopmans@ieee.org)
 *
 * This file is part of paparazzi.
 *
 * paparazzi is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * paparazzi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with paparazzi; see the file COPYING.  If not, write to
 * the Free Software Foundation, 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/**
 * @file test_telemetry.c
 *
 * Periodically sends ALIVE telemetry messages.
 */


/* ChibiOS includes */
#include "ch.h"

/* paparazzi includes */
#include "mcu.h"
#include "mcu_periph/sys_time.h"
#include "mcu_periph/uart.h"
#include "subsystems/datalink/downlink.h"
#include "led.h"

/*
 * Red LEDs blinker thread, times are in milliseconds.
 */
static WORKING_AREA(waThdBlinker, 128);
static msg_t ThdBlinker(void *arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (TRUE) {
#ifdef SYS_TIME_LED
    LED_TOGGLE(SYS_TIME_LED);
#endif
    chThdSleepMilliseconds(500);
  }
  return 0;
}


int main(void)
{
  mcu_init();

  downlink_init();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThdBlinker, sizeof(waThdBlinker), NORMALPRIO, ThdBlinker, NULL);

  while (TRUE) {
    DOWNLINK_SEND_ALIVE(DefaultChannel, DefaultDevice, 16, MD5SUM);

#ifdef LED_GREEN
    LED_TOGGLE(LED_GREEN);
#endif
    chThdSleep(MS2ST(100));
  }

  return 0;
}
