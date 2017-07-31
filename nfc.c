#include "ch.h"
#include "hal.h"
#include "trace.h"
#include "trf797x.h"

static struct Trf797xInitiatorDriver driver;

static struct Trf797xInitiatorConfig config = {
        .spi = &SPID1,
        .event = 1,
        .proto = TRF7970X_PROTO_RFID_14443A_106,
        .gpio = {
                .en = GPIO_EN,
                .io = {
                        GPIO_IO0, GPIO_IO1, GPIO_IO2, GPIO_IO3,
                        GPIO_IO4, GPIO_IO5, GPIO_IO6, GPIO_IO7
                }
        }
};

void trf797x_extcallback(EXTDriver *extp, expchannel_t channel) {
    (void) extp; (void) channel;
    chSysLockFromISR();
    tf797x_interrupt_hookI((Trf797xDriver *)&driver);
    chSysUnlockFromISR();
}

static THD_WORKING_AREA(waNfcThread, 512);

static THD_FUNCTION(NfcThread, arg) {
    (void)arg;

    uint8_t rxbuf[16];
    bool led = TRUE;

    struct trf797x_tx tx = {
            .buf = "\x26",    // REQA
            .bits = 7,
    };

    struct trf797x_rx rx = {
            .buf = rxbuf,
            .len = sizeof(rxbuf),
    };

    chRegSetThreadName("nfc");
    extChannelEnable(&TRF797X_EXT_DRIVER, TRF797X_EXT_CHANNEL);

    while (true) {

        if(led)
            gpioSetPad(GPIO_LED1);
        else
            gpioClearPad(GPIO_LED1);

        if(trf797x_initiator_start(&driver, &config) == 0) {

            // Let target power-up
            chThdSleepMilliseconds(5);

            int len = trf797x_initiator_transceive(&driver, &tx, &rx, MS2ST(200));
            if(len > 0) {
                trace("ATQA = %02X%02X", rxbuf[0], rxbuf[1]);
            }else
                chThdSleepMilliseconds(800);

            trf797x_initiator_stop(&driver, TRUE);
        }

        chThdSleepMilliseconds(100);
        led = !led;
    }
};

void nfcStart() {
    chThdCreateStatic(waNfcThread, sizeof(waNfcThread), HIGHPRIO, NfcThread, NULL);
}
