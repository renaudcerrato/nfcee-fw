#include "trf797x.h"
#include "trf797x_lld.h"

#define BIT(n)                          (1 << (n))
#define CMD_BIT_CTRL                    BIT(7)
#define CMD_BIT_READ                    BIT(6)
#define CMD_BIT_CONTINUOUS              BIT(5)
#define CMD_BIT_OPCODE(opcode)          ((opcode) & 0x1f)

#define REGISTER(adr, len)              (CMD_BIT_OPCODE(adr) | ((len) > 1 ? CMD_BIT_CONTINUOUS : 0))
#define REGISTER_WRITE(adr, len)        REGISTER(adr, len)
#define REGISTER_READ(adr, len)         (CMD_BIT_READ | REGISTER(adr, len))
#define COMMAND(cmd)                    (CMD_BIT_CTRL | CMD_BIT_OPCODE(cmd))

#ifndef NS2RTC
#define NS2RTC(freq, nsec)		        (rtcnt_t)(((nsec) * (freq/1000UL) + 999999UL)/1000000UL)
#endif

#ifndef WAIT_NANOSECOND
#define WAIT_NANOSECOND(n)              osalSysPolledDelayX(NS2RTC(SystemCoreClock, n))
#endif

#ifndef MIN
#define MIN(a, b)                       ((a) < (b) ? (a) : (b))
#endif

#define TIMING_NSS_RISING_TO_FALLING_EDGE               300     // nanoseconds
#define TIMING_NSS_FALLING_EDGE_TO_CLOCK                200     // nanoseconds
#define TIMING_CLOCK_TO_NSS_RISING_EDGE                 200     // nanoseconds

#define TOGGLE_CHIP_SELECT(spi)             do{                                                     \
                                                WAIT_NANOSECOND(TIMING_CLOCK_TO_NSS_RISING_EDGE);   \
                                                spiUnselect(spi);                                   \
                                                WAIT_NANOSECOND(TIMING_NSS_RISING_TO_FALLING_EDGE); \
                                                spiSelect(spi);                                     \
                                                WAIT_NANOSECOND(TIMING_NSS_FALLING_EDGE_TO_CLOCK);  \
                                            }while(0)

#define read_fifo_bytes(spi)               (trf797x_register_read1(spi, TRF797X_REG_FIFO_STATUS)     \
                                                & ~TRF797X_FIFO_STATUS_OVERFLOW)


void trf797x_acquire_bus(SPIDriver *spi) {
    spiAcquireBus(spi);
    WAIT_NANOSECOND(TIMING_NSS_RISING_TO_FALLING_EDGE);
    spiSelect(spi);
    WAIT_NANOSECOND(TIMING_NSS_FALLING_EDGE_TO_CLOCK);
}

void trf797x_release_bus(SPIDriver *spi) {
    WAIT_NANOSECOND(TIMING_CLOCK_TO_NSS_RISING_EDGE);
    spiUnselect(spi);
    spiReleaseBus(spi);
}

void trf797x_command(SPIDriver *spi, trf797x_cmd_t cmd) {
    spi_lld_polled_exchange(spi,  COMMAND(cmd));
}

size_t trf797x_register_read(SPIDriver *spi, trf797x_reg_t adr, void *data, size_t len) {

    // Dummy read required to clear IRQ.
    if(adr == TRF797X_REG_IRQ_STATUS && len == 1) {
        uint8_t tmp[2];
        trf797x_register_read(spi, adr, tmp, 2);
        if(data) *(uint8_t *)data = tmp[0];
    }else {
        spi_lld_polled_exchange(spi, REGISTER_READ(adr, len));

        if(data)
            spiReceive(spi, len, data);
        else
            spiIgnore(spi, len);
    }

    if(len > 1) {
        TOGGLE_CHIP_SELECT(spi);
    }

    return len;
}

void trf797x_register_write(SPIDriver *spi, trf797x_reg_t adr, const void *data, size_t len) {
    spi_lld_polled_exchange(spi, REGISTER_WRITE(adr, len));
    spiSend(spi, len, data);

    if(len > 1) {
        TOGGLE_CHIP_SELECT(spi);
    }
}

size_t trf797x_transmit(SPIDriver *spi, const struct trf797x_iovec **tx, size_t *len, bool crc) {

    size_t fifo = 0, sent;
    int bits = 0;

    for(size_t i = 0; i < *len; i++) {
        bits+=(*tx)[i].bits;
    }

    const uint8_t header[] = {
            COMMAND(TRF797X_CMD_RESET_FIFO),
            COMMAND(crc ? TRF797X_CMD_TX : TRF797X_CMD_TX_NO_CRC),
            REGISTER_WRITE(TRF797X_REG_TX_LENGTH1, 2 + MIN(TRF797X_FIFO_SIZE, (bits + 7) / 8)),
            (uint8_t) (bits >> 7),
            (uint8_t) (((bits & 0xFF) << 1) | ((bits % 8) != 0))
    };

    spiSend(spi, sizeof(header), header);

    while(bits > 0 && fifo < TRF797X_FIFO_SIZE) {
        const size_t bytes = ((*tx)->bits + 7) / 8;
        sent = MIN(bytes, TRF797X_FIFO_SIZE - fifo);

        spiSend(spi, sent, (*tx)->base);

        bits-=(*tx)->bits;
        fifo+=sent;

        if(sent == bytes) {
            if(--(*len)) {
                (*tx)++;
            }
        }
    }

    TOGGLE_CHIP_SELECT(spi);

    return sent;
}

int trf797x_fifo_fill(SPIDriver *spi, const void *data, size_t len) {
    const int fifo_bytes = TRF797X_FIFO_SIZE - read_fifo_bytes(spi);
    len = MIN(len, fifo_bytes);
    if (len > 0) {
        trf797x_register_write(spi, TRF797X_REG_FIFO, data, len);
    }
    return len;
}

int trf797x_fifo_drain(SPIDriver *spi, void *data, size_t len) {

    int fifo_bytes = read_fifo_bytes(spi);

    if(data && len > 0) {
        len = MIN(len, fifo_bytes);

        if(len > 0) {
            trf797x_register_read(spi, TRF797X_REG_FIFO, data, len);
            fifo_bytes-=len;
        }
    }else {
        len = 0;
    }

    if(fifo_bytes > 0) {
        trf797x_register_read(spi, TRF797X_REG_FIFO, NULL, fifo_bytes);
    }

    return len;
}
