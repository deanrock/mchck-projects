#include <mchck.h>
#include <usb/usb.h>
#include <usb/cdc-acm.h>

static struct cdc_ctx cdc;

volatile uint8_t diodes[25*3];
volatile int current = 0;
volatile int time = 0;

static void
new_data(uint8_t *data, size_t len)
{
        if(time>10) {
                current = 0;
        }
        time = 0;

        for (; len > 0; ++data, --len) {
                diodes[current++] = *data;

                if (current >= 25*3) {
                        current = 0;
                        gpio_toggle(GPIO_PTB16);

                        for(int i = 0; i < 25*3; i++) {
                                uint8_t d = diodes[i];

                                for (uint8_t j = 8; j > 0; j--) {
                                        d<<=1;
                                        gpio_write(GPIO_PTB0, d&(1<<7) ? GPIO_HIGH : GPIO_LOW);
                                        gpio_write(GPIO_PTB1, GPIO_HIGH);
                                        gpio_write(GPIO_PTB1, GPIO_LOW);
                                }
                        }
                }
        }

        cdc_read_more(&cdc);
}

static void
init_vcdc(int config)
{
        cdc_init(new_data, NULL, &cdc);
}

static const struct usbd_device cdc_device =
        USB_INIT_DEVICE(0x2323,              /* vid */
                        3,                   /* pid */
                        u"deanrock.co",        /* vendor */
                        u"LED pixels", /* product" */
                        (init_vcdc,          /* init */
                         CDC)               /* functions */
                );

int
main(void)
{
        usb_init(&cdc_device);

        gpio_dir(GPIO_PTB16, GPIO_OUTPUT);
        pin_mode(GPIO_PTB16, PIN_MODE_DRIVE_HIGH);

        gpio_dir(GPIO_PTB0, GPIO_OUTPUT); /* data */
        gpio_dir(GPIO_PTB1, GPIO_OUTPUT); /* clock */

        pin_mode(GPIO_PTB0, PIN_MODE_DRIVE_LOW);
        pin_mode(GPIO_PTB1, PIN_MODE_DRIVE_LOW);

        for (;;) {
                for (volatile int i = 10000; i > 0; --i)
                        /* NOTHING */;
                
                time++;
        }
}

