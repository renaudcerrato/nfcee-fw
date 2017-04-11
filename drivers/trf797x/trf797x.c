#include "trf797x.h"
#include "trf797x_gpio.h"

#define EVENT_IRQ               (1 << 0)
#define EVENT_STOP              (1 << 31)

void trf797x_driver_init(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_IDLE;
    osalEventObjectInit(&driver->event);
}

bool trf797x_init(Trf797xDriver *driver) {

    CLEAR(EN);

#ifdef TRF797X_HAVE_EN2
    CLEAR(EN2);
#endif

#ifdef TRF797X_HAVE_IO0
    CLEAR(IO0);
#endif

#ifdef TRF797X_HAVE_IO1
    SET(IO1);
#endif

#ifdef TRF797X_HAVE_IO2
    SET(IO2);
#endif

    CLEAR(CS);
    chThdSleepMilliseconds(2);
    SET(CS);
    chThdSleepMilliseconds(4);
    SET(EN);
    chThdSleepMilliseconds(5); // how much?
    //TODO: initialization
}

void tf797x_interrupt_hookI(Trf797xDriver *driver) {
    osalSysLockFromISR();
    osalEventBroadcastFlagsI(&driver->event, EVENT_IRQ);
    osalSysUnlockFromISR();
}

void trf797x_stop(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_STOP;
    osalEventBroadcastFlags(&driver->event, EVENT_STOP);
}