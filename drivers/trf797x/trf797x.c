#include "trf797x.h"


void trf797x_init(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_IDLE;
    driver->thread = NULL;
}

void tf797x_interrupt_hookI(Trf797xDriver *driver) {
    osalSysLockFromISR();
    osalThreadResumeI(&driver->thread, MSG_OK);
    osalSysUnlockFromISR();
}

void trf797x_stop(Trf797xDriver *driver) {
    driver->state = TRF797XA_ST_STOP;
    osalThreadResumeS(&driver->thread, MSG_RESET);
}