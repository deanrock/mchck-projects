enum i2c_mode {
        I2C_MODE_MASTER = 0 << 0,
        I2C_MODE_SLAVE = 0 << 1,
};

void i2c_init(void);
void i2c_begin(enum i2c_mode mode, uint8_t address);
