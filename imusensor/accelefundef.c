#include "icm20602.h"

// SPI Write Function
int spi_write_register(int spi_fd, uint8_t reg_addr, uint8_t value) {
    uint8_t tx_buffer[2] = {reg_addr & 0x7F, value}; // Clear MSB for write
    if (write(spi_fd, tx_buffer, sizeof(tx_buffer)) != sizeof(tx_buffer)) {
        perror("SPI Write Error");
        return -1;
    }
    return 0;
}

// SPI Read Function
int spi_read_register(int spi_fd, uint8_t reg_addr, uint8_t *data) {
    uint8_t tx_buffer = reg_addr | 0x80; // Set MSB for read
    uint8_t rx_buffer = 0;

    // Send register address
    if (write(spi_fd, &tx_buffer, 1) != 1) {
        perror("SPI Write Error (Read Command)");
        return -1;
    }

    // Read data
    if (read(spi_fd, &rx_buffer, 1) != 1) {
        perror("SPI Read Error");
        return -1;
    }

    *data = rx_buffer;
    return 0;
}

// Configure Accelerometer for ±4g
int configure_accelerometer(int spi_fd) {
    uint8_t config;
    if (spi_read_register(spi_fd, ACCEL_CONFIG_REG, &config) < 0) {
        return -1;
    }
    config = (config & ~ACCEL_FS_SEL_MASK) | ACCEL_FS_SEL_4G;
    return spi_write_register(spi_fd, ACCEL_CONFIG_REG, config);
}

// Read Accelerometer Data
void read_accelerometer(int spi_fd, float *x_g, float *y_g, float *z_g) {
    uint8_t high_byte, low_byte;
    int16_t x_raw, y_raw, z_raw;

    // Read X-axis
    spi_read_register(spi_fd, ACCEL_XOUT_H, &high_byte);
    spi_read_register(spi_fd, ACCEL_XOUT_L, &low_byte);
    x_raw = (int16_t)((high_byte << 8) | low_byte);

    // Read Y-axis
    spi_read_register(spi_fd, ACCEL_YOUT_H, &high_byte);
    spi_read_register(spi_fd, ACCEL_YOUT_L, &low_byte);
    y_raw = (int16_t)((high_byte << 8) | low_byte);

    // Read Z-axis
    spi_read_register(spi_fd, ACCEL_ZOUT_H, &high_byte);
    spi_read_register(spi_fd, ACCEL_ZOUT_L, &low_byte);
    z_raw = (int16_t)((high_byte << 8) | low_byte);

    // Convert raw values to g
    *x_g = x_raw / SENSITIVITY_4G;
    *y_g = y_raw / SENSITIVITY_4G;
    *z_g = z_raw / SENSITIVITY_4G;
}
// Configure Gyroscope  for ±500dps
int configure_gyroscope(int spi_fd) {
    uint8_t config_gyro;
    if (spi_read_register(spi_fd, GYRO_CONFIG_REG, &config_gyro) < 0) {
        return -1;
    }
    config_gyro = (config_gyro & ~GYRO_FS_SEL_MASK) | GYRO_FS_SEL_500dps;
    return spi_write_register(spi_fd, GYRO_CONFIG_REG, config_gyro);
}

// Read Gyroscope  Data
void read_gyroscope(int spi_fd, float *x_dps, float *y_dps, float *z_dps) {
    uint8_t high_bytegyro, low_bytegyro;
    int16_t x_gyro, y_gyro, z_gyro;

    // Read X-axis
    spi_read_register(spi_fd, GYRO_XOUT_H, &high_bytegyro);
    spi_read_register(spi_fd, GYRO_XOUT_L, &low_bytegyro);
    x_gyro = (int16_t)((high_bytegyro << 8) | low_bytegyro);

    // Read Y-axis
    spi_read_register(spi_fd, GYRO_YOUT_H, &high_bytegyro);
    spi_read_register(spi_fd, GYRO_YOUT_L, &low_bytegyro);
    y_gyro = (int16_t)((high_bytegyro << 8) | low_bytegyro);

    // Read Z-axis
    spi_read_register(spi_fd, GYRO_ZOUT_H, &high_bytegyro);
    spi_read_register(spi_fd, GYRO_ZOUT_L, &low_bytegyro);
    z_gyro = (int16_t)((high_bytegyro << 8) | low_bytegyro);

    // Convert raw values to dpa
    *x_dps = x_gyro / SENSITIVITY_500dps;
    *y_dps = y_gyro / SENSITIVITY_500dps;
    *z_dps = z_gyro / SENSITIVITY_500dps;
}

