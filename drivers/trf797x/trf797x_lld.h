#ifndef TRF797X_LLD_H
#define TRF797X_LLD_H

/** TRF797x Registers */
#define TRF797X_REG_CHIP_STATUS             0x00
#define TRF797X_REG_ISO_CTRL                0x01
#define TRF797X_REG_ISO14443B_TX            0x02
#define TRF797X_REG_ISO14443_BITRATE        0x03
#define TRF797X_REG_TIMER_TX_MSB            0x04
#define TRF797X_REG_TIMER_TX_LSB            0x05
#define TRF797X_REG_TX_PULSE_LENGTH         0x06
#define TRF797X_REG_NO_RESPONSE_WAIT_TIME   0x07
#define TRF797X_REG_RX_WAIT_TIME            0x08
#define TRF797X_REG_MODULATOR_SYS_CLK       0x09
#define TRF797X_REG_RX_SPECIAL              0x0a
#define TRF797X_REG_REGULATOR_CTRL          0x0b
#define TRF797X_REG_IRQ_STATUS              0x0c
#define TRF797X_REG_IRQ_MASK                0x0d
#define TRF797X_REG_COLLISION_POSITION      0x0e
#define TRF797X_REG_RSSI_LEVELS             0x0f
#define TRF797X_REG_SPECIAL_FUNCTIONS       0x10
#define TRF797X_REG_FIFO_IRQ_LEVELS         0x14
#define TRF797X_REG_NFC_LOW_FIELD_LEVEL     0x16
#define TRF797X_REG_NFCID1                  0x17
#define TRF797X_REG_NFC_TARGET_DETECTION    0x18
#define TRF797X_REG_NFC_TARGET_PROTOCOL     0x19
#define TRF797X_REG_TEST1                   0x1a
#define TRF797X_REG_TEST2                   0x1b
#define TRF797X_REG_FIFO_STATUS             0x1c
#define TRF797X_REG_TX_LENGTH1              0x1d
#define TRF797X_REG_TX_LENGTH2              0x1e
#define TRF797X_REG_FIFO                    0x1f

/** TRF797x Commands */
#define TRF797X_CMD_IDLE                0x00
#define TRF797X_CMD_INIT                0x03
#define TRF797X_CMD_RESET_FIFO          0x0f
#define TRF797X_CMD_TX_NO_CRC           0x10
#define TRF797X_CMD_TX                  0x11
#define TRF797X_CMD_TX_NO_CRC_DELAYED   0x12
#define TRF797X_CMD_TX_DELAYED          0x13
#define TRF797X_CMD_EOF_TX_NEXT_SLOT    0x14
#define TRF797X_CMD_RX_DISABLE          0x16
#define TRF797X_CMD_RX_ENABLE           0x17
#define TRF797X_CMD_TEST_INTERNAL_RF    0x18
#define TRF797X_CMD_TEST_EXTERNAL_RF    0x19

typedef uint8_t trf797x_reg_t;
typedef uint8_t trf797x_cmd_t;

typedef struct PACKED_VAR {
    trf797x_reg_t adr;
    uint8_t value;
} trf797x_reg_map_t;

void trf797x_command(SPIDriver *spi, trf797x_cmd_t cmd);
size_t trf797x_register_read(SPIDriver *spi, trf797x_reg_t adr, void *data, size_t len);
void trf797x_register_write(SPIDriver *spi, trf797x_reg_t adr, const void *data, size_t len);
void trf797x_register_write_map(SPIDriver *spi, trf797x_reg_map_t *map, size_t len);

static inline uint8_t trf797x_register_read1(SPIDriver *spi, trf797x_reg_t reg) {
    uint8_t val;
    trf797x_register_read(spi, reg, &val, 1);
    return val;
}

static inline void trf797x_register_write1(SPIDriver *spi, trf797x_reg_t reg, uint8_t value) {
    trf797x_register_write(spi, reg, &value, 1);
}

#endif //TRF797X_LLD_H