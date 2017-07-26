#ifndef TRF797X_LLD_H
#define TRF797X_LLD_H

#define TRF797X_FIFO_SIZE                  127
#define TRF797X_SPI_CLK_FREQ_MAX           1000000UL

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

/* Modulator and SYS_CLK Control Register Bits */
#define TRF7970X_MODULATOR_DEPTH(n)             ((n) & 0x7)
#define TRF7970X_MODULATOR_DEPTH_ASK10          (TRF7970X_MODULATOR_DEPTH(0))
#define TRF7970X_MODULATOR_DEPTH_OOK            (TRF7970X_MODULATOR_DEPTH(1))
#define TRF7970X_MODULATOR_DEPTH_ASK7           (TRF7970X_MODULATOR_DEPTH(2))
#define TRF7970X_MODULATOR_DEPTH_ASK8_5         (TRF7970X_MODULATOR_DEPTH(3))
#define TRF7970X_MODULATOR_DEPTH_ASK13          (TRF7970X_MODULATOR_DEPTH(4))
#define TRF7970X_MODULATOR_DEPTH_ASK16          (TRF7970X_MODULATOR_DEPTH(5))
#define TRF7970X_MODULATOR_DEPTH_ASK22          (TRF7970X_MODULATOR_DEPTH(6))
#define TRF7970X_MODULATOR_DEPTH_ASK30          (TRF7970X_MODULATOR_DEPTH(7))
#define TRF7970X_MODULATOR_EN_ANA               BIT(3)
#define TRF7970X_MODULATOR_CLK(n)               (((n) & 0x3) << 4)
#define TRF7970X_MODULATOR_EN_OOK               BIT(6)
#define TRF7970X_MODULATOR_27MHZ                BIT(7)


/* IRQ Status Register Bits */
#define TRF7970X_IRQ_STATUS_NORESP              BIT(0) /* ISO15693 only */
#define TRF7970X_IRQ_STATUS_NFC_COL_ERROR       BIT(0)
#define TRF7970X_IRQ_STATUS_COL                 BIT(1)
#define TRF7970X_IRQ_STATUS_FRAMING_EOF_ERROR   BIT(2)
#define TRF7970X_IRQ_STATUS_NFC_RF              BIT(2)
#define TRF7970X_IRQ_STATUS_PARITY_ERROR        BIT(3)
#define TRF7970X_IRQ_STATUS_NFC_SDD             BIT(3)
#define TRF7970X_IRQ_STATUS_CRC_ERROR           BIT(4)
#define TRF7970X_IRQ_STATUS_NFC_PROTO_ERROR     BIT(4)
#define TRF7970X_IRQ_STATUS_FIFO                BIT(5)
#define TRF7970X_IRQ_STATUS_SRX                 BIT(6)
#define TRF7970X_IRQ_STATUS_TX                  BIT(7)

#define TRF7970X_IRQ_STATUS_ERROR                                   \
                 (TRF7970X_IRQ_STATUS_COL |                         \
                  TRF7970X_IRQ_STATUS_FRAMING_EOF_ERROR |           \
                  TRF7970X_IRQ_STATUS_PARITY_ERROR |                \
                  TRF7970X_IRQ_STATUS_CRC_ERROR)

/* Chip Status Control Register Bits */
#define TRF797X_CHIP_STATUS_VRS5_3             BIT(0)
#define TRF797X_CHIP_STATUS_REC_ON             BIT(1)
#define TRF797X_CHIP_STATUS_AGC_ON             BIT(2)
#define TRF797X_CHIP_STATUS_PM_ON              BIT(3)
#define TRF797X_CHIP_STATUS_RF_PWR             BIT(4)
#define TRF797X_CHIP_STATUS_RF_ON              BIT(5)
#define TRF797X_CHIP_STATUS_DIRECT             BIT(6)
#define TRF797X_CHIP_STATUS_STBY               BIT(7)

/* ISO Control Register Bits */
#define TRF7970X_ISO_CTRL_NFC_NFCA_106          0x01
#define TRF7970X_ISO_CTRL_NFC_NFCF_212          0x02
#define TRF7970X_ISO_CTRL_NFC_NFCF_424          0x03
#define TRF7970X_ISO_CTRL_NFC_CE_14443A         0x00
#define TRF7970X_ISO_CTRL_NFC_CE_14443B         0x01
#define TRF7970X_ISO_CTRL_NFC_CE                BIT(2)
#define TRF7970X_ISO_CTRL_NFC_ACTIVE            BIT(3)
#define TRF7970X_ISO_CTRL_NFC_INITIATOR         BIT(4)
#define TRF7970X_ISO_CTRL_NFC_NFC_CE_MODE       BIT(5)
#define TRF7970X_ISO_CTRL_RFID                  BIT(5)
#define TRF7970X_ISO_CTRL_DIR_MODE              BIT(6)
#define TRF7970X_ISO_CTRL_RX_CRC_N              BIT(7)  /* true == No CRC */

#define TRF797X_FIFO_STATUS_OVERFLOW            BIT(7)

#define TRF797X_FIFO_IRQ_LEVELS_WLH_124	        (0x0 << 2)
#define TRF797X_FIFO_IRQ_LEVELS_WLH_120	        (0x1 << 2)
#define TRF797X_FIFO_IRQ_LEVELS_WLH_112	        (0x2 << 2)
#define TRF797X_FIFO_IRQ_LEVELS_WLH_96	        (0x3 << 2)
#define TRF797X_FIFO_IRQ_LEVELS_WLL_4	        0x0
#define TRF797X_FIFO_IRQ_LEVELS_WLL_8	        0x1
#define TRF797X_FIFO_IRQ_LEVELS_WLL_16	        0x2
#define TRF797X_FIFO_IRQ_LEVELS_WLL_32	        0x3

typedef uint8_t trf797x_reg_t;
typedef uint8_t trf797x_cmd_t;


void trf797x_acquire_bus(SPIDriver *spi);

void trf797x_command(SPIDriver *spi, trf797x_cmd_t cmd);

size_t trf797x_register_read(SPIDriver *spi, trf797x_reg_t adr, void *data, size_t len);
void trf797x_register_write(SPIDriver *spi, trf797x_reg_t adr, const void *data, size_t len);

int trf797x_transmit(SPIDriver *spi, const void *data, size_t bits, bool crc);
int trf797x_fifo_fill(SPIDriver *spi, const void *data, size_t len);
int trf797x_fifo_drain(SPIDriver *spi, void *data, size_t len);

void trf797x_release_bus(SPIDriver *spi);

static inline uint8_t trf797x_register_read1(SPIDriver *spi, trf797x_reg_t reg) {
    uint8_t val;
    trf797x_register_read(spi, reg, &val, 1);
    return val;
}

static inline void trf797x_register_write1(SPIDriver *spi, trf797x_reg_t reg, uint8_t value) {
    trf797x_register_write(spi, reg, &value, 1);
}

#endif //TRF797X_LLD_H