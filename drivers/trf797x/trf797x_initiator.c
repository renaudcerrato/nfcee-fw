#include "trf797x.h"
#include "trf797x_lld.h"

#define _rf(drv, on)            do{                                                                             \
                                    trf797x_register_write1(drv->config->spi, TRF797X_REG_CHIP_STATUS,          \
                                    ((on) ? TRF797X_CHIP_STATUS_RF_ON : 0) |                                    \
                                    (drv->config->vin == TRF7970X_VIN_5V ? TRF797X_CHIP_STATUS_VRS5_3 : 0));    \
                                }while(0)

#define rf_on(drv)              _rf(drv, TRUE)
#define rf_off(drv)             _rf(drv, FALSE)

#define read_irq(drv)           trf797x_register_read1(drv->config->spi, TRF797X_REG_IRQ_STATUS)

#define __CLEANUP__(m)          __attribute__ ((__cleanup__(m)))
#define ACQUIRE_FOR_SCOPE(spi)  SPIDriver *__spi __CLEANUP__(do_release) = spi;                                 \
                                trf797x_acquire_bus(__spi);

#ifndef MIN
#define MIN(a, b)                ((a) < (b) ? (a) : (b))
#endif

static void do_release(SPIDriver **spi) {
    trf797x_release_bus(*spi);
}

static int irq2error(uint8_t irq) {
    if(irq & TRF7970X_IRQ_STATUS_COL) return TRF797X_ERR_COLLISION;
    if(irq & TRF7970X_IRQ_STATUS_CRC_ERROR) return TRF797X_ERR_CHECKSUM;
    if(irq & TRF7970X_IRQ_STATUS_FRAMING_EOF_ERROR) return TRF797X_ERR_FRAMING;
    if(irq & TRF7970X_IRQ_STATUS_PARITY_ERROR) return TRF797X_ERR_PARITY;
    return 0;
}

void trf797x_initiator_driver_init(Trf797xInitiatorDriver *driver) {
    trf797x_driver_init((Trf797xDriver *) driver);
}

int trf797x_initiator_start(Trf797xInitiatorDriver *drv, const Trf797xInitiatorConfig *config) {

    ACQUIRE_FOR_SCOPE(config->spi);

    const int result = trf797x_start((Trf797xDriver *) drv, (const Trf797xConfig *) config);
    if(result < 0) return result;

    // Mask anything but errors and FIFO
    trf797x_register_write1(config->spi, TRF797X_REG_IRQ_MASK,
                            TRF7970X_IRQ_STATUS_ERROR | TRF7970X_IRQ_STATUS_FIFO);

    // Keep CRC into the RX buffer
    trf797x_register_write1(config->spi, TRF797X_REG_ISO_CTRL,
                            config->proto | TRF7970X_ISO_CTRL_RFID | TRF7970X_ISO_CTRL_RX_CRC_N |
                            TRF7970X_ISO_CTRL_DIR_MODE);

    // FIFO levels
    trf797x_register_write1(config->spi, TRF797X_REG_FIFO_IRQ_LEVELS,
                            TRF797X_FIFO_IRQ_LEVELS_WLH_96 | TRF797X_FIFO_IRQ_LEVELS_WLL_32);

    // Power-up the target
    rf_on(drv);
    chThdSleepMilliseconds(5); //TODO: defer until transceive

    return 0;
}

int trf797x_initiator_transceive(Trf797xInitiatorDriver *drv, const struct trf797x_transfer *tr) {

    ACQUIRE_FOR_SCOPE(drv->config->spi);

    eventflags_t flags;
    uint8_t irq;

    const void *tx_buf = tr->txbuf;
    void *rx_buf = tr->rxbuf;
    int tx_bytes = (tr->txbits + 7) / 8;
    int rx_bytes = (tr->rxbits + 7) / 8;

    // Clear IRQ flags
    read_irq(drv);

    // Clear event flags
    chEvtGetAndClearFlags(&drv->listener);

    // Fill FIFO & transmit
    int len = trf797x_transmit(drv->config->spi, tx_buf, tr->txbits, FALSE);

    tx_buf+=len;
    tx_bytes-=len;

    // Continuous transmit
    do {
        // Wait for IRQ
        if (chEvtWaitAnyTimeout(EVENT_MASK(drv->config->event), TIME_INFINITE) == 0) {
            return TRF797X_ERR_TIMEOUT;
        }

        flags = chEvtGetAndClearFlags(&drv->listener);
        if (flags & EVENT_STOP) return TRF797X_ERR_CANCELLED;

        irq = read_irq(drv);

        if (irq & TRF7970X_IRQ_STATUS_FIFO) {
            if (tx_bytes > 0) {
                len = trf797x_fifo_fill(drv->config->spi, tx_buf, tx_bytes);
                tx_buf+=len;
                tx_bytes-=len;
            }
        }

    }while(irq != TRF7970X_IRQ_STATUS_TX);

    // Continuous receive
    do {
        // Wait for IRQ
        if (chEvtWaitAnyTimeout(EVENT_MASK(drv->config->event), tr->timeout) == 0) {
            return TRF797X_ERR_TIMEOUT;
        }

        flags = chEvtGetAndClearFlags(&drv->listener);
        if (flags & EVENT_STOP) return TRF797X_ERR_CANCELLED;

        irq = read_irq(drv);

        if(irq & TRF7970X_IRQ_STATUS_ERROR) {
            return irq2error(irq);
        }

        if(irq & TRF7970X_IRQ_STATUS_FIFO) {
            len = trf797x_fifo_drain(drv->config->spi, rx_buf, rx_bytes);
            if(len > 0) {
                rx_buf+=len;
                rx_bytes-=len;
            }
        }

    }while(irq != TRF7970X_IRQ_STATUS_SRX);

    return (int) (rx_buf - tr->rxbuf);
}

void trf797x_initiator_stop(Trf797xInitiatorDriver *drv, bool shutdown) {

    trf797x_stop((Trf797xDriver *) drv, shutdown);

    if(!shutdown) {
        ACQUIRE_FOR_SCOPE(drv->config->spi);
        rf_off(drv);
        drv->state = TRF797X_ST_RF_OFF;
    }
}