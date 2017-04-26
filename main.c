#include "ch.h"
#include "hal.h"
#include "gpio.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    gpioSetPad(GPIO_LED0);
    chThdSleepMilliseconds(500);
    gpioClearPad(GPIO_LED0);
    chThdSleepMilliseconds(500);
  }
}

void testInit(void)
{
  /*
   * Creates the example thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);
}

int main(void) {

  halInit();
  chSysInit();

  testInit();

  while (true) {
    chThdSleepMilliseconds(500);
  }
}


