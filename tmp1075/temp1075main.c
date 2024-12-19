// main.c

#include "temp1075.h"

int main() {
    // Open I2C bus
    int i2c_fd = open_i2c_bus();
    if (i2c_fd < 0) {
        perror("Failed to open I2C bus");
        return 1;
    }

    // Set the TMP1075 I2C address
    if (set_i2c_address(i2c_fd, TMP1075_I2C_ADDRESS) < 0) {
        perror("Failed to set I2C address");
        close(i2c_fd);
        return 1;
    }

    // Select the temperature register for to read
    char reg = TEMP_REGISTER;
    if (write_register(i2c_fd, reg) < 0) {
        perror("Failed to write register address");
        close(i2c_fd);
        return 1;
    }

    // Read 2 bytes of temperature data
    char data[2];
    if (read_temperature(i2c_fd, data) < 0) {
        perror("Failed to read temperature data");
        close(i2c_fd);
        return 1;
    }

    // Close the I2C bus
    close(i2c_fd);

    // Convert the raw data to temperature in Celsius
    int raw_temp = (data[0] << 8 | data[1]) >> 4;
  if(raw_temp & 0x800) //to check for negative num whether msb is set or reset
 {
	 raw_temp |=0xF000;// if the number is negative invert and make 2's complement
 }

	  float temperature = convert_to_temperature(raw_temp);

    // Print the temperature
    printf("Temperature: %.2f°C\n", temperature);

    return 0;
}

