#include "ch.h"
#include "hal.h"
#include "trace.h"

/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waBlinker, 128);
static THD_FUNCTION(Blinker, arg) {

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

int main(void) {

    halInit();
    chSysInit();

    trace_init();

    trace("[NFCEE] - Hello world\r\n");

    // Blinker
    chThdCreateStatic(waBlinker, sizeof(waBlinker), NORMALPRIO, Blinker, NULL);
    // NFC Demo
    nfcStart();

    while (true) {
        chThdSleepMilliseconds(500);
    }
}


