#include "icm20602.h"

int main() {
    int spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("Failed to open SPI device");
        return -1;
    }

    // Set SPI Mode
    uint8_t spi_mode = SPI_MODE;
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &spi_mode) < 0) {
        perror("Failed to set SPI mode");
        close(spi_fd);
        return -1;
    }

    // Set SPI Speed
    uint32_t spi_speed_hz = SPI_SPEED_HZ;
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &spi_speed_hz) < 0) {
        perror("Failed to set SPI speed");
        close(spi_fd);
        return -1;
    }

    // Verify WHO_AM_I Register
    uint8_t who_am_i;
    if (spi_read_register(spi_fd, WHO_AM_I_REG, &who_am_i) < 0 || who_am_i != EXPECTED_WHO_AM_I) {
        fprintf(stderr, "Device not detected. WHO_AM_I: 0x%X\n", who_am_i);
        close(spi_fd);
        return -1;
    }
    printf("ICM20602 detected. WHO_AM_I: 0x%X\n", who_am_i);

    // Configure Accelerometer
    if (configure_accelerometer(spi_fd) < 0) {
        fprintf(stderr, "Failed to configure accelerometer\n");
        close(spi_fd);
        return -1;
    }

    // Read and Print Accelerometer Data
    float x_g, y_g, z_g;
    read_accelerometer(spi_fd, &x_g, &y_g, &z_g);
    printf("Accelerometer Readings:\n");
    printf("X: %.3f g\n", x_g);
    printf("Y: %.3f g\n", y_g);
    printf("Z: %.3f g\n", z_g);

     // Configure Gyroscope
    if (configure_gyroscope(spi_fd) < 0) {
        fprintf(stderr, "Failed to configure accelerometer\n");
        close(spi_fd);
        return -1;
    }

    // Read and Print Accelerometer Data
    float x_dps, y_dps, z_dps;
    read_gyroscope(spi_fd, &x_dps, &y_dps, &z_dps);
    printf("Gyroscope  Readings:\n");
    printf("X: %.3f g\n", x_dps);
    printf("Y: %.3f g\n", y_dps);
    printf("Z: %.3f g\n", z_dps);

   // Close SPI Device
    close(spi_fd);
    return 0;
}

