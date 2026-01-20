#include <zephyr.h>
#include <device.h>
#include <drivers/gpio.h>
#include <sys/printk.h>

/* ดึงข้อมูล LED จาก Device Tree Alias 'led0' */
#define LED0_NODE DT_ALIAS(led0)

#if DT_NODE_HAS_STATUS(LED0_NODE, okay)
#define LED0_LABEL  DT_GPIO_LABEL(LED0_NODE, gpios)
#define LED0_PIN    DT_GPIO_PIN(LED0_NODE, gpios)
#define LED0_FLAGS  DT_GPIO_FLAGS(LED0_NODE, gpios)
#else
#error "Unsupported board: led0 devicetree alias is not defined"
#endif

/* ช่วงเวลาหน่วง (มิลลิวินาที) */
#define SLEEP_TIME_MS   500

void main(void)
{
    const struct device *dev;
    bool led_is_on = true;
    int ret;

    /* ดึง Device Binding ของ GPIO */
    dev = device_get_binding(LED0_LABEL);
    if (dev == NULL) {
        printk("Error: Look up of %s failed\n", LED0_LABEL);
        return;
    }

    /* ตั้งค่า Pin ให้เป็น Output */
    ret = gpio_pin_configure(dev, LED0_PIN, GPIO_OUTPUT_ACTIVE | LED0_FLAGS);
    if (ret < 0) {
        printk("Error: gpio_pin_configure failed (ret %d)\n", ret);
        return;
    }

    printk("SparkFun nRF9160 Blink started (NCS v1.4.1)\n");

    while (1) {
        /* สลับสถานะไฟ */
        gpio_pin_set(dev, LED0_PIN, (int)led_is_on);
        led_is_on = !led_is_on;

        k_msleep(SLEEP_TIME_MS);
    }
}