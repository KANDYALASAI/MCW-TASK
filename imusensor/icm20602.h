#ifndef ICM20602_H
#define ICM20602_H

#include <stdint.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

// SPI Configuration
#define SPI_DEVICE            "/dev/spidev0.0"  // SPI device path
#define SPI_SPEED_HZ          1000000           // 1 MHz SPI clock speed
#define SPI_MODE              SPI_MODE_0        // SPI mode

// ICM20602 Registers for who am i and accelerometer,gyroscope

#define WHO_AM_I_REG          0x75              // WHO_AM_I register
#define EXPECTED_WHO_AM_I     0x12              // Expected WHO_AM_I value for ICM20602
#define ACCEL_CONFIG_REG      0x1C              // Accelerometer configuration register
#define ACCEL_FS_SEL_MASK     0x18              // Mask to clear FS_SEL bits
#define ACCEL_FS_SEL_4G       0x08              // Value to configure accelerometer to ±4g
#define ACCEL_XOUT_H          0x3B              // X-axis high byte register
#define ACCEL_XOUT_L          0x3C              // X-axis low byte register
#define ACCEL_YOUT_H          0x3D              // Y-axis high byte register
#define ACCEL_YOUT_L          0x3E              // Y-axis low byte register
#define ACCEL_ZOUT_H          0x3F              // Z-axis high byte register
#define ACCEL_ZOUT_L          0x40              // Z-axis low byte register
#define SENSITIVITY_4G        8192.0f              // Sensitivity factor for ±4g

#define GYRO_CONFIG_REG      0x1B               // Gyroscope Configuration register
#define GYRO_FS_SEL_MASK     0x18               //  Mask to clear FS_SEL bits
#define GYRO_FS_SEL_500dps   0x08               // value to configure gyroscope to ±500dps
#define GYRO_XOUT_H          0x43               //x-axis high byte register
#define GYRO_XOUT_L          0x44               // x-axis low byte register
#define GYRO_YOUT_H	     0x45	        // y-axis high byte register
#define GYRO_YOUT_L	     0x46		// y-axis low byte register
#define GYRO_ZOUT_H	     0x47		// z-axiz high byte register
#define GYRO_ZOUT_L	     0x48		// z-axis low byte register
#define SENSITIVITY_500dps   65.5f		// Sensitivity factor for ±500dps

// Function Prototypes
int spi_write_register(int spi_fd, uint8_t reg_addr, uint8_t value);
int spi_read_register(int spi_fd, uint8_t reg_addr, uint8_t *data);
int configure_accelerometer(int spi_fd);
void read_accelerometer(int spi_fd, float *x_g, float *y_g, float *z_g);
int configure_gyroscope(int spi_fd);
void read_gyroscope(int spi_fd, float *x_dps, float *y_dps, float *z_dps);

#endif // ICM20602_H

