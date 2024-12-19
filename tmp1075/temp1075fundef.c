#include "temp1075.h"

int open_i2c_bus() {
    int i2c_fd = open("/dev/i2c-1", O_RDWR);
    if (i2c_fd < 0) {
        perror("Failed to open I2C bus");
        return -1;
    }
    return i2c_fd;
}

int set_i2c_address(int i2c_fd, int address) {
    if (ioctl(i2c_fd, I2C_SLAVE, address) < 0) {
        perror("Failed to set I2C address");
        return -1;
    }
    return 0;
}

int write_register(int i2c_fd, char reg) {
    if (write(i2c_fd, &reg, 1) != 1) {
        perror("Failed to write register address");
        return -1;
    }
    return 0;
}

int read_temperature(int i2c_fd, char *data) {
    if (read(i2c_fd, data, 2) != 2) {
        perror("Failed to read temperature data");
        return -1;
    }
    return 0;
}

float convert_to_temperature(int raw_temp) {
    return raw_temp * 0.0625;
}


