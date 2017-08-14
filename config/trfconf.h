#ifndef TRFCONF_H
#define TRFCONF_H

/**
 * @brief TRF797X SPI clock
 */
#define TRF797X_CONF_SPI_CLOCK         1000000L

/**
 * @brief TR797X Oscillator: 27Mhz/13.56Mhz
 */
#define TRF797X_CONF_27MHZ_OSC          1

/**
 * @brief SYS_CLK pin output divider
 * @note valid values in [0, 1, 2, 4]
 */
#define TRF797X_CONF_SYS_CLK_DIV        0

/**
 * @brief Vin Operation range
 */
#define TRF797X_CONF_VIN_5V             1

/**
 * @brief enable libnfc parts
 */
#define TRF797X_CONF_HAVE_LIBNFC        1

#endif // TRFCONF_H
