#include "trf797x.h"
#include "trf797x_lld.h"

#define BIT(n)                          (1 << (n))
#define CMD_BIT_CTRL                    BIT(7)
#define CMD_BIT_READ                    BIT(6)
#define CMD_BIT_CONTINUOUS              BIT(5)
#define CMD_BIT_OPCODE(opcode)          ((opcode) & 0x1f)

#define REGISTER(adr, cnt)              (CMD_BIT_OPCODE(adr) | ((cnt) > 1 ? CMD_BIT_CONTINUOUS : 0))
#define REGISTER_WRITE(adr, cnt)        REGISTER(adr, cnt)
#define REGISTER_READ(adr, cnt)         (CMD_BIT_READ | REGISTER(adr, cnt))
#define COMMAND(cmd)                    (CMD_BIT_CTRL | CMD_BIT_OPCODE(cmd))

#ifndef WAIT_NANOSECOND
#warning "[TODO] WAIT_NANOSECOND is not defined"
#define WAIT_NANOSECOND(n)
#endif

#define TIMING_NSS_RISING_TO_FALLING_EDGE               300     // nanoseconds
#define TIMING_NSS_FALLING_EDGE_TO_CLOCK                200     // nanoseconds
#define TIMING_CLOCK_TO_NSS_RISING_EDGE                 200     // nanoseconds

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
        *(uint8_t *)data = tmp[0];
    }else {
        spi_lld_polled_exchange(spi, REGISTER_READ(adr, len));
        spiReceive(spi, len, data);
    }

    return len;
}

void trf797x_register_write(SPIDriver *spi, trf797x_reg_t adr, const void *data, size_t len) {
    spi_lld_polled_exchange(spi, REGISTER_WRITE(adr, len));
    spiSend(spi, len, data);
}

void trf797x_register_write_map(SPIDriver *spi, trf797x_reg_map_t *map, size_t len) {
    spiSend(spi, len*sizeof(trf797x_reg_map_t), map);
}

void trf797x_command_transmit(SPIDriver *spi, const void *data, size_t bits, bool crc) {
    const uint8_t txlength[2] = { (uint8_t) (bits >> 7),  (uint8_t) (((bits & 0xFF) << 1) | ((bits % 8) != 0))};
    trf797x_command(spi, crc ? TRF797X_CMD_TX : TRF797X_CMD_TX_NO_CRC);
    trf797x_register_write(spi, TRF797X_REG_TX_LENGTH1, &txlength, 2);
    spiSend(spi, (bits + 7) / 8, data);
}