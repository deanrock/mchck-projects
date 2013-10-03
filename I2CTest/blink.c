#include <mchck.h>
#include "i2c.h"


void write_out_byte(uint8_t b) {
        gpio_mode(GPIO_PTD7, GPIO_MODE_OUTPUT|GPIO_MODE_DRIVE_LOW);
        
        gpio_write(GPIO_PTD7, GPIO_HIGH);
        gpio_write(GPIO_PTD7, GPIO_LOW);
        int i;
        for(i = 0;i<8;i++,b<<=1) {
                gpio_write(GPIO_PTD7, b&(1<<7) ? GPIO_HIGH : GPIO_LOW);
        }
        gpio_write(GPIO_PTD7, GPIO_HIGH);
        gpio_write(GPIO_PTD7, GPIO_LOW);
}

void check_addr(uint8_t addr)
{
        I2C0_C1 |= I2C_C1_TX_MASK; //master - address cycle
        I2C0_C1 |= I2C_C1_MST_MASK;//master mode & send start signal

        I2C0_D = (addr<<1) | 0x00; //data - address, read request
        //I2C0_C1 &= ~(1<<4); //receive

        while (!(I2C0_S & I2C_S_BUSY_MASK));

        while ((I2C0_S & I2C_S_IICIF_MASK) == 0) {
                //
        }//wait
        I2C0_S |= I2C_S_IICIF_MASK;

        if(!(I2C0_S & I2C_S_RXAK_MASK)) {
                gpio_toggle(GPIO_PTB16);
        }

        I2C0_C1 &= ~I2C_C1_MST_MASK;

        for (volatile int i = 10000; i > 0; --i)
                /* NOTHING */;
}



void i2c_send(uint8_t address, uint8_t *data, int size)
{
        I2C0_C1 |= I2C_C1_TX_MASK; //master - address cycle
        I2C0_C1 |= I2C_C1_MST_MASK;//master mode & send start signal

        I2C0_D = (address<<1) | 0x00; //data - address, write request

        while (!(I2C0_S & I2C_S_BUSY_MASK));

        while ((I2C0_S & I2C_S_IICIF_MASK) == 0);//wait for interrupt
        
        I2C0_S |= I2C_S_IICIF_MASK;
        
        if(!(I2C0_S & I2C_S_RXAK_MASK)) {
                //device ack'd
                int i = 0;
                for(i = 0;i<size;i++)
                {
                        I2C0_C1 |= I2C_C1_TX_MASK; //transmit data
                        I2C0_D = data[i];
                        //while (!(I2C0_S & I2C_S_TCF_MASK));//wait
                        while ((I2C0_S & I2C_S_IICIF_MASK) == 0) {
                                //
                        }//wait
                        I2C0_S |= I2C_S_IICIF_MASK;
                }
        }

        I2C0_C1 &= ~I2C_C1_MST_MASK;//stop master, return to slave mode
        
        for (volatile int i = 10000; i > 0; --i)
                /* NOTHING */;
}

void i2c_receive(uint8_t address, uint8_t* data, int size)
{
        I2C0_C1 |= I2C_C1_TX_MASK; //master - address cycle
        I2C0_C1 |= I2C_C1_MST_MASK;//master mode & send start signal

        I2C0_D = (address<<1) | 0x01; //data - address, read request

        while (!(I2C0_S & I2C_S_BUSY_MASK));

        while ((I2C0_S & I2C_S_IICIF_MASK) == 0);//wait for interrupt
        
        I2C0_S |= I2C_S_IICIF_MASK;
        
        if(!(I2C0_S & I2C_S_RXAK_MASK)) {
                //device ack'd
                
        /*      I2C0_C1 |= I2C_C1_RSTA_MASK;
                I2C0_D = (address<<1) | 0x01; //data - address, read request
                while ((I2C0_S & I2C_S_IICIF_MASK) == 0);//wait for interrupt
        
        I2C0_S |= I2C_S_IICIF_MASK;
        */
        
                int i = 0;
                for(i = 0;i<size;i++)
                {
                        
                        I2C0_C1 &= ~I2C_C1_TX_MASK; //
                        data[i] = I2C0_D;
                        while ((I2C0_S & I2C_S_IICIF_MASK) == 0) {
                                //
                        }//wait
                        I2C0_S |= I2C_S_IICIF_MASK;
                }
        }

        I2C0_C1 &= ~I2C_C1_MST_MASK;//stop master, return to slave mode
        
        for (volatile int i = 10000; i > 0; --i)
                /* NOTHING */;
}



int
main(void)
{
        
        for (;;) {
                        
                //for (volatile int i = 1000000; i > 0; --i)
                //              /* NOTHING */;
                //              onboard_led(ONBOARD_LED_TOGGLE);
                gpio_mode(GPIO_PTB16, GPIO_MODE_OUTPUT | GPIO_MODE_DRIVE_HIGH);
                gpio_mode(GPIO_PTB3, GPIO_MODE_OUTPUT | GPIO_MODE_DRIVE_HIGH);
                ///write_out_byte(0x06);
                //write_out_byte(120);
                gpio_toggle(GPIO_PTB3);

                i2c_init();

                i2c_begin(I2C_MODE_MASTER, 0x52);

                
                check_addr(0x52);
                //check_addr(103);
                //check_addr(104);
                //check_addr(105);
                //check_addr(29);
                //check_addr(83);
                
                
                //initiate nunchuck
                uint8_t c[2] = {0x40, 0x00};
                //uint8_t c[100] = {0xF0};
                i2c_send(0x52, c, 2);
                
                //request data
                uint8_t c2[1] = {0x00};
                i2c_send(0x52, &c2, 1);
                
                //receive data
                uint8_t buf[6];
                i2c_receive(0x52, buf, 6);//read 6 bytes
                

                                        

        }       
}

