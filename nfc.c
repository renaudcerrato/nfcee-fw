#include <nfc.h>
#include <libs/nfc/include/iso14443.h>
#include "ch.h"
#include "hal.h"
#include "trace.h"
#include "trf797x.h"


static struct trf797x_nfc_device device = {

        .config = {
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
        }
};

static void trace_nfcid(struct nfcid *id) {
    trace("\nNFC_ID = ");
    for(int i = 0; i < id->len; i++) {
        trace("%02X", id->data[i]);
    }
}

void trf797x_extcallback(EXTDriver *extp, expchannel_t channel) {
    (void) extp; (void) channel;
    chSysLockFromISR();
    tf797x_interrupt_hookI((Trf797xDriver *)&device.driver);
    chSysUnlockFromISR();
}

static THD_WORKING_AREA(waNfcThread, 512);

static void raw_demo() {

    uint8_t rxbuf[16];

    bool led = TRUE;

    struct trf797x_iovec tx = {
            .base = "\x26",    // REQA
            .bits = 7,
    };

    struct trf797x_iovec rx = {
            .base = rxbuf,
            .bytes = sizeof(rxbuf),
    };

    trf797x_initiator_driver_init(&device.driver);

    while (true) {

        if(led)
            gpioSetPad(GPIO_LED1);
        else
            gpioClearPad(GPIO_LED1);

        if(trf797x_initiator_start(&device.driver, &device.config) == 0) {

            // Give some time to power-up target(s)
            chThdSleepMilliseconds(5);

            int len = trf797x_initiator_transceive(&device.driver, &tx, 1, &rx, MS2ST(200));
            if(len > 0) {
                trace("ATQA = %02X%02X", rxbuf[0], rxbuf[1]);
            }else
                chThdSleepMilliseconds(800);

            trf797x_initiator_stop(&device.driver, TRUE);
        }

        chThdSleepMilliseconds(100);
        led = !led;
    }
}

static void libnfc_demo() {

    nfc_iso14443a_driver_t driver;
    bool led = TRUE;

    // Initialize TRF7970 driver structure.
    trf797x_nfc_device_init(&device);

    // Initialize NFC driver structure.
    nfc_driver_t *nfc = nfc_iso14443a_driver_init(&driver, (struct nfc_device *) &device);

    while (true) {

        if(led)
            gpioSetPad(GPIO_LED1);
        else
            gpioClearPad(GPIO_LED1);

        if(nfc_open(nfc) == 0) {

            if(nfc->tech == NFC_DIGITAL_RF_14443A || nfc->tech == NFC_DIGITAL_RF_14443B) {
                trace_nfcid(&((nfc_iso14443_driver_t *)nfc)->nfcid);
            }

            nfc_close(nfc);

            chThdSleepMilliseconds(150);
        }else
            chThdSleepMilliseconds(500);

        led = !led;
    }
}

static THD_FUNCTION(NfcThread, arg) {

    chRegSetThreadName("nfc");
    extChannelEnable(&TRF797X_EXT_DRIVER, TRF797X_EXT_CHANNEL);

    if((int) arg)
        libnfc_demo();
    else
        raw_demo();
};

void nfcStart() {
    chThdCreateStatic(waNfcThread, sizeof(waNfcThread), HIGHPRIO, NfcThread, (void *) FALSE);
}
