#include "trf797x.h"

#define EVENT_IRQ       (1 << 0)
#define EVENT_STOP      (1 << 31)

void trf797x_init(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_IDLE;
    osalEventObjectInit(&driver->event);
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