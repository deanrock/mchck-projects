#include <mchck.h>
#include "i2c.h"

void
i2c_init(void)
{
        SIM.scgc4.i2c0 = 1; //enable I2C clock
        SIM.scgc5.portb = 1; //enable PORTB clock

        //alternate function 2
        PORTB.pcr[0].mux = PCR_MUX_ALT2;
        PORTB.pcr[1].mux = PCR_MUX_ALT2;
}

void
i2c_begin(enum i2c_mode mode, uint8_t address)
{
        //multiplier = 1, clockrate (ICR) = 17
        I2C0.f.icr = 18;
        I2C0.f.mult = SPI_MULT_4;

        I2C0.c1.iicen = 1; //enable I2C
        I2C0.c1.iicie = 1; //enable interrupts

        if (mode == I2C_MODE_MASTER) {

        }
}