/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-06-05     tanek        first implementation.
 * 2018-10-10     Ernest Chen  update and improve capability
 * 2018-11-11     Ernest Chen  add sharing SDA with different SCL
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>
#include "drv_gpio.h"
#include "drv_i2c.h"

//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINTF(...) rt_kprintf(__VA_ARGS__)
#else
#define DEBUG_PRINTF(...)
#endif

/*user can change this*/
#define I2CBUS_NAME 	"i2c1"
#define I2CBUS_NAME_S 	"i2c2"//aht10 standby name

/*user should change this to adapt specific board*/
#define  IO_I2C_SCL_PORT                HT_GPIOC
#define  IO_I2C_SCL_PIN                 Pin_13
#define  IO_I2C_SCL_INPUT               HD_System_GPIO_DIR(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN, PUPDR_UP, INPUT)
#define  IO_I2C_SCL_OUTPUT              HD_System_GPIO_DIR(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN, PUPDR_UP, OUTPUT)
#define  IO_I2C_SCL_H                   HD_System_GPIO_DATA(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN, 1)
#define  IO_I2C_SCL_L                   HD_System_GPIO_DATA(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN, 0)
#define  IO_I2C_SCL_PV                  HD_System_GPIO_Read(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN)
#define  IO_I2C_SCL_DIR                 HD_System_GPIO_DIR_Read(IO_I2C_SCL_PORT, IO_I2C_SCL_PIN)

#define  IO_I2C_SDA_PORT                HT_GPIOC
#define  IO_I2C_SDA_PIN                 Pin_14
#define  IO_I2C_SDA_INPUT               HD_System_GPIO_DIR(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN, PUPDR_UP, INPUT)
#define  IO_I2C_SDA_OUTPUT              HD_System_GPIO_DIR(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN, PUPDR_UP, OUTPUT)
#define  IO_I2C_SDA_OD_OUTPUT           HD_System_GPIO_DIR(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN, PUPDR_OD, OUTPUT)
#define  IO_I2C_SDA_H                   HD_System_GPIO_DATA(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN, 1)
#define  IO_I2C_SDA_L                   HD_System_GPIO_DATA(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN, 0)
#define  IO_I2C_SDA_PV                  HD_System_GPIO_Read(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN)
#define  IO_I2C_SDA_DIR                 HD_System_GPIO_DIR_Read(IO_I2C_SDA_PORT, IO_I2C_SDA_PIN)


#ifdef BSP_USING_I2C

static struct rt_i2c_bus_device i2c2_bus;

static void i2c_gpio_init()
{
	IO_I2C_SCL_H;IO_I2C_SCL_OUTPUT;IO_I2C_SCL_H;
    IO_I2C_SDA_H;IO_I2C_SDA_OUTPUT;IO_I2C_SDA_H;
}

static void set_sda(void *data, rt_int32_t state)
{
	IO_I2C_SDA_OUTPUT;
    if (state)
    {
        IO_I2C_SDA_H;
    }
    else
    {
        IO_I2C_SDA_L;
    }
}

static void set_scl(void *data, rt_int32_t state)
{
	IO_I2C_SCL_OUTPUT;
    if (state)
    {
        IO_I2C_SCL_H;
    }
    else
    {
        IO_I2C_SCL_L;
    }
}

static rt_int32_t get_sda(void *data)
{
	IO_I2C_SDA_INPUT;
    if (IO_I2C_SDA_PV)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static rt_int32_t get_scl(void *data)
{
	IO_I2C_SCL_INPUT;
    if (IO_I2C_SCL_PV)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

static void udelay(rt_uint32_t us)
{
    rt_uint32_t ticks;
    rt_uint32_t told, tnow, tcnt = 0;
    rt_uint32_t reload = SysTick->LOAD;

    ticks = us * reload / (1000000 / RT_TICK_PER_SECOND);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
            {
                tcnt += told - tnow;
            }
            else
            {
                tcnt += reload - tnow + told;
            }
            told = tnow;

            if (tcnt >= ticks)
            {
                break;
            }
        }
    }
}

static const struct rt_i2c_bit_ops bit_ops =
{
    RT_NULL,
    set_sda,
    set_scl,
    get_sda,
    get_scl,
    udelay,
    10,
    10
};

/* if i2c is locked, the function will unlock it  */
static rt_err_t i2c_bus_unlock()
{
    rt_int32_t i = 0;

    if (0 == get_sda(NULL))
    {
        while (i++ < 9)
        {
            set_scl(NULL, 0); //reset clk
            udelay(100);
            set_scl(NULL, 1);
            udelay(100);
        }
    }
    if (0 == get_sda(NULL))
    {
        return -RT_ERROR;
    }

    return RT_EOK;
}

int hw_i2c_init(void)
{
    i2c_gpio_init();

    if (i2c_bus_unlock() != RT_EOK)
    {
        rt_kprintf("Failed to unlock i2c \n");
        return -RT_ERROR;
    }

    rt_memset((void *)&i2c2_bus, 0, sizeof(struct rt_i2c_bus_device));

    i2c2_bus.priv = (void *)&bit_ops;

    rt_i2c_bit_add_bus(&i2c2_bus, I2CBUS_NAME);

    return RT_EOK;
}
INIT_BOARD_EXPORT(hw_i2c_init);

#endif /* end of i2c driver */
