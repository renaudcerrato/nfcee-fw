#include "trf797x.h"
#include "trf797x_lld.h"

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
    if(irq & TRF7970X_IRQ_STATUS_COL) return -TRF797X_ERR_COLLISION;
    if(irq & TRF7970X_IRQ_STATUS_CRC_ERROR) return -TRF797X_ERR_CHECKSUM;
    if(irq & TRF7970X_IRQ_STATUS_FRAMING_EOF_ERROR) return -TRF797X_ERR_FRAMING;
    if(irq & TRF7970X_IRQ_STATUS_PARITY_ERROR) return -TRF797X_ERR_PARITY;
    return 0;
}


static int switch_rf(const Trf797xInitiatorConfig *config, bool on) {
    //TODO: check RF level
    trf797x_register_write1(config->spi, TRF797X_REG_CHIP_STATUS,
                            ((on) ? TRF797X_CHIP_STATUS_RF_ON : 0) |
                            ((TRF797X_CONF_VIN_5V) ? TRF797X_CHIP_STATUS_VRS5_3 : 0));
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
    trf797x_register_write1(config->spi, TRF797X_REG_ISO_CTRL, config->proto | TRF7970X_ISO_CTRL_RX_CRC_N);

    // FIFO levels
    trf797x_register_write1(config->spi, TRF797X_REG_FIFO_IRQ_LEVELS,
                            TRF797X_FIFO_IRQ_LEVELS_WLH_96 | TRF797X_FIFO_IRQ_LEVELS_WLL_32);

    return switch_rf(config, TRUE);
}

int trf797x_initiator_transceive(Trf797xInitiatorDriver *drv, const struct trf797x_iovec *tx, size_t txlen, const struct trf797x_iovec *rx, systime_t timeout) {

    if(txlen == 0) return -EINVAL;

    ACQUIRE_FOR_SCOPE(drv->config->spi);

    eventflags_t flags;
    uint8_t irq;

    // Clear IRQ flags
    read_irq(drv);

    // Clear event flags
    chEvtGetAndClearFlags(&drv->listener);

    // Fill FIFO & transmit
    int len = trf797x_transmit(drv->config->spi, &tx, &txlen, FALSE);

    const void *tx_buf = tx->base + len;
    size_t tx_bytes = (tx->bits + 7) / 8 - len;

    void *rx_buf = rx->base;
    size_t rx_bytes = (rx->bits + 7) / 8;

    // Continuous transmit
    do {
        // Wait for IRQ
        if (chEvtWaitAnyTimeout(EVENT_MASK(drv->config->event), TIME_INFINITE) == 0) {
            return -ETIMEDOUT;
        }

        flags = chEvtGetAndClearFlags(&drv->listener);
        if (flags & EVENT_STOP) return -ECANCELED;

        irq = read_irq(drv);

        if (irq & TRF7970X_IRQ_STATUS_FIFO) {

            if (tx_bytes == 0 && txlen) {
                tx++;
                tx_buf = tx->base;
                tx_bytes = (tx->bits + 7) / 8;
            }

            if(tx_bytes > 0) {
                len = trf797x_fifo_fill(drv->config->spi, tx_buf, tx_bytes);
                if(len > 0) {
                    tx_buf+=len;
                    tx_bytes-=len;
                }
            }
        }

    }while(irq != TRF7970X_IRQ_STATUS_TX);

    // Continuous receive
    do {
        // Wait for IRQ
        if (chEvtWaitAnyTimeout(EVENT_MASK(drv->config->event), timeout) == 0) {
            return -ETIMEDOUT;
        }

        flags = chEvtGetAndClearFlags(&drv->listener);
        if (flags & EVENT_STOP) return -ECANCELED;

        irq = read_irq(drv);

        if(irq & TRF7970X_IRQ_STATUS_ERROR) {
            return irq2error(irq);
        }

        if(irq & (TRF7970X_IRQ_STATUS_FIFO | TRF7970X_IRQ_STATUS_SRX)) {
            len = trf797x_fifo_drain(drv->config->spi, rx_buf, rx_bytes);
            if(len > 0) {
                rx_buf+=len;
                rx_bytes-=len;
            }
        }

    }while(irq != TRF7970X_IRQ_STATUS_SRX);

    return (int) (rx_buf - rx->base);
}

int trf797x_initiator_stop(Trf797xInitiatorDriver *drv, bool shutdown) {

    trf797x_stop((Trf797xDriver *) drv, shutdown);

    if(!shutdown) {
        ACQUIRE_FOR_SCOPE(drv->config->spi);
        switch_rf(drv->config, FALSE);
        drv->state = TRF797X_ST_RF_OFF;
    }

    return 0;
}