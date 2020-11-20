/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     zylx         first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "fal.h"
#include "easyflash.h"

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "main"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

#define WDT_FEED_PERIOD 250
#define WDT_DEVICE_NAME "wdt"
static rt_device_t wdg_dev;

static void idle_hook(void)
{
    static rt_uint32_t last_feed_time = 0;

    if(rt_tick_get() - last_feed_time > WDT_FEED_PERIOD)
    {
        last_feed_time = rt_tick_get();
        rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    }
}

void wdt_init(void)
{
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    rt_device_init(wdg_dev);
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    rt_thread_idle_sethook(idle_hook);
}

void led_thread_entry(void* parameter)
{
#define interval 250

    rt_pin_mode(IO_RUN_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);

    while (1)
    {
        rt_pin_write(IO_RUN_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RUN_LED, PIN_HIGH);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);

        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_TX_LED, PIN_HIGH);
        rt_thread_mdelay(interval);
    }
}

/*
int set_console_baud(int argc, char **argv)
{
    rt_device_t console_dev;
    struct serial_configure cfg = RT_SERIAL_CONFIG_DEFAULT;

    if(argc == 2)
    {
        sscanf(argv[1], "%d", &cfg.baud_rate);
    }
    console_dev = rt_device_find("uart0");
    if(console_dev != RT_NULL)
    {
        rt_device_control(console_dev, RT_DEVICE_CTRL_CONFIG, &cfg);
    }

    return 0;
}
MSH_CMD_EXPORT_ALIAS(set_console_baud, baud, set baudrate usage: baud 115200);
*/

int main(void)
{
    rt_thread_t led_thread_ptr;
    rt_uint32_t reboot_cnt = 0, len = 0;
    char reboot_cnt_buf[11];

    wdt_init();
    fal_init();
    easyflash_init();

    ef_get_env_blob("reboot_cnt", reboot_cnt_buf, sizeof(reboot_cnt_buf), &len);
    reboot_cnt = atoi(reboot_cnt_buf);
    reboot_cnt++;
    rt_sprintf(reboot_cnt_buf, "%d", reboot_cnt);
    ef_set_env("reboot_cnt", reboot_cnt_buf);

    led_thread_ptr = rt_thread_create("led",
                                    led_thread_entry, RT_NULL,
                                    256, 5, 1);

    if (led_thread_ptr != RT_NULL) rt_thread_startup(led_thread_ptr);

    return RT_EOK;
}
