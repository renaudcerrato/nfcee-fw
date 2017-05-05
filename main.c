#include "ch.h"
#include "hal.h"
#include "gpio.h"
#include "trace.h"
#if defined(USB_CDC_TRACE)
#include "usbcfg.h"
#endif

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

	int i = 0;
    (void)arg;
    chRegSetThreadName("blinker");
    while (true) {
		trace("[NFCEE] - count %d\r\n", i++);
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

	trace_init();
    testInit();
    trace("[NFCEE] - Hello world\r\n");
    while (true) {
        chThdSleepMilliseconds(500);
    }
}


