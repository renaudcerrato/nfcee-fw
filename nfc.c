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

static THD_FUNCTION(NfcThread, arg) {

    chRegSetThreadName("nfc");
    extChannelEnable(&TRF797X_EXT_DRIVER, TRF797X_EXT_CHANNEL);

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
};

void nfcStart() {
    chThdCreateStatic(waNfcThread, sizeof(waNfcThread), HIGHPRIO, NfcThread, NULL);
}
