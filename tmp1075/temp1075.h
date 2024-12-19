// temp1075.h

#ifndef TEMP1075_H
#define TEMP1075_H

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>

#define TMP1075_I2C_ADDRESS 0x48 //  I2C address
#define TEMP_REGISTER 0x00       // Temperature register

int open_i2c_bus();
int set_i2c_address(int i2c_fd, int address);
int write_register(int i2c_fd, char reg);
int read_temperature(int i2c_fd, char *data);
float convert_to_temperature(int raw_temp);

#endif // TMP1075_H

