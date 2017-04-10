#include "trf797x.h"

#define BIT(n)                          (1 << (n))
#define CMD_BIT_CTRL                    BIT(7)
#define CMD_BIT_READ                    BIT(6)
#define CMD_BIT_CONTINUOUS              BIT(5)
#define CMD_BIT_OPCODE(opcode)          ((opcode) & 0x1f)

#define REGISTER(adr, cnt)              (CMD_BIT_OPCODE(adr) | ((cnt) > 1 ? CMD_BIT_CONTINUOUS : 0))
#define REGISTER_WRITE(adr, cnt)        REGISTER(adr, cnt)
#define REGISTER_READ(adr, cnt)         (CMD_BIT_READ | REGISTER(adr, cnt))
#define COMMAND(cmd)                    (CMD_BIT_CTRL | CMD_BIT_OPCODE(cmd))

void trf797x_command(SPIDriver *spi, trf797x_cmd_t cmd) {
    spiSelect(spi);
    spi_lld_polled_exchange(spi,  COMMAND(cmd));
    spiUnselect(spi);
}

size_t trf797x_register_read(SPIDriver *spi, trf797x_reg_t adr, void *data, size_t len) {

    // Dummy read required to clear IRQ...
    if(adr == TRF797X_REG_IRQ_STATUS && len == 1) {
        uint8_t tmp[2];
        trf797x_register_read(spi, adr, tmp, 2);
        *(uint8_t *)data = tmp[0];
    }else {
        spiSelect(spi);
        spi_lld_polled_exchange(spi, REGISTER_READ(adr, len));
        spiReceive(spi, len, data);
        spiUnselect(spi);
    }

    return len;
}

void trf797x_register_write(SPIDriver *spi, trf797x_reg_t adr, const void *data, size_t len) {
    spiSelect(spi);
    spi_lld_polled_exchange(spi, REGISTER_WRITE(adr, len));
    spiSend(spi, len, data);
    spiUnselect(spi);
}

void trf797x_register_write_map(SPIDriver *spi, trf797x_reg_map_t *map, size_t len) {
    spiSelect(spi);
    spiSend(spi, len*sizeof(trf797x_reg_map_t), map);
    spiUnselect(spi);
}