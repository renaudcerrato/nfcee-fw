#include "trf797x.h"
#include "trf797x_lld.h"

#define _rf(cfg, on)   do{                                                                      \
                            trf797x_register_write1(cfg->spi, TRF797X_REG_CHIP_STATUS,          \
                            ((on) ? TRF7970A_CHIP_STATUS_RF_ON : 0) |                           \
                            (cfg->vin == TRF7970X_VIN_5V ? TRF7970A_CHIP_STATUS_VRS5_3 : 0));   \
                        }while(0)

#define rf_on(cfg)      _rf(cfg, TRUE)
#define rf_off(cfg)     _rf(cfg, FALSE)

#define read_irq(cfg)   trf797x_register_read1(cfg->spi, TRF797X_REG_IRQ_STATUS)

static int irq2error(uint8_t irq) {
    if(irq & TRF7970X_IRQ_STATUS_COL) return TRF797X_ERR_COLLISION;
    if(irq && TRF7970X_IRQ_STATUS_CRC_ERROR) return TRF797X_ERR_CHECKSUM;
    if(irq && TRF7970X_IRQ_STATUS_FRAMING_EOF_ERROR) return TRF797X_ERR_FRAMING;
    if(irq && TRF7970X_IRQ_STATUS_PARITY_ERROR) return TRF797X_ERR_PARITY;
    return 0;
}

void trf797x_initiator_driver_init(Trf797xInitiatorDriver *driver) {
    trf797x_driver_init((Trf797xDriver *) driver);
}

int trf797x_initiator_start(Trf797xInitiatorDriver *drv, const Trf797xInitiatorConfig *config) {

    trf797x_acquire_bus(config->spi);

    const int result = trf797x_start((Trf797xDriver *) drv, (const Trf797xConfig *) config);
    if(result < 0) goto err;

    // Mask anything but errors
    trf797x_register_write1(config->spi, TRF797X_REG_IRQ_MASK, TRF7970X_IRQ_STATUS_ERROR);

    // Keep CRC into the RX buffer
    trf797x_register_write1(config->spi, TRF797X_REG_ISO_CTRL, config->proto |
                            TRF7970X_ISO_CTRL_RFID | TRF7970X_ISO_CTRL_RX_CRC_N |
                            TRF7970X_ISO_CTRL_DIR_MODE);

    rf_on(config); // TODO: some delay is required for the target to power-up

err:
    trf797x_release_bus(config->spi);
    return result;
}

int trf797x_initiator_transceive(Trf797xInitiatorDriver *drv, struct trf797x_transfer *tr) {

    eventflags_t flags;
    uint8_t irq;

    // Clear IRQ flags
    read_irq(drv->config);

    // Reset FIFO & transmit
    trf797x_command(drv->config->spi, TRF797X_CMD_RESET_FIFO);
    trf797x_command_transmit(drv->config->spi, tr->txbuf, tr->txbits, FALSE);

    // Wait for end of TX
    if(chEvtWaitAnyTimeout(EVENT_MASK(drv->config->event), TIME_INFINITE) == 0) {
        return TRF797X_ERR_TIMEOUT;
    }

    flags = chEvtGetAndClearFlags(&drv->listener);

    if(flags & EVENT_STOP) {
       return TRF797X_ERR_PROBE;
    }

    irq = read_irq(drv->config);

    if(irq & TRF7970X_IRQ_STATUS_TX) {
        //TODO: wait for RX interrupt
    }

    //TODO: read fifo
    return 0;
}

void trf797x_initiator_stop(Trf797xInitiatorDriver *drv, bool shutdown) {

    if(!shutdown) {
        rf_off(drv->config);
        drv->state = TRF797X_ST_RF_OFF;
    }

    trf797x_stop((Trf797xDriver *) drv, shutdown);
    trf797x_release_bus(drv->config->spi);
}