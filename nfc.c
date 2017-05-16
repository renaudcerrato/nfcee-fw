#include "ch.h"
#include "hal.h"
#include "trace.h"
#include "trf797x.h"

static struct Trf797xInitiatorDriver driver;

static struct Trf797xInitiatorConfig config = {
        .spi = &SPID1,
        .event = 1,
        .proto = TRF7970X_PROTO_RFID_14443A_106,
        .div = TRF7970X_SYS_CLK_DISABLED,
        .osc27m = FALSE,
        .vin = TRF7970X_VIN_5V,
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

    struct trf797x_transfer tr = {
            .txbuf = "\x26",    // REQA
            .txbits = 7,
            .rxbuf = rxbuf,
            .rxbits = sizeof(rxbuf)*8,
            .timeout = MS2ST(200)
    };

    chRegSetThreadName("nfc");
    extChannelEnable(&TRF797X_EXT_DRIVER, TRF797X_EXT_CHANNEL);

    while (true) {

        if(trf797x_initiator_start(&driver, &config) == 0) {

            int len = trf797x_initiator_transceive(&driver, &tr);
            if(len > 0) {
                trace("REQB!");
            }

            trf797x_initiator_stop(&driver, TRUE);
        }

        chThdSleepMilliseconds(200);
    }
};

void nfcStart() {
    chThdCreateStatic(waNfcThread, sizeof(waNfcThread), HIGHPRIO, NfcThread, NULL);
}
