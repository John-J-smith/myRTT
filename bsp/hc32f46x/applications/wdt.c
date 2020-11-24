/*
 * Copyright (c) Aug.2,2004  R&D Institute of Reallin Co., Ltd.
 *
 * watch dog driver
 *
 * Change Logs:
 * Date           Author       Notes
 * 2020-11-23     John Shi     first version
 */

#include <rtthread.h>
#include <rtdevice.h>

#define WDT_FEED_PERIOD 250 //ms
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

static int wdt_init(void)
{
    wdg_dev = rt_device_find(WDT_DEVICE_NAME);
    rt_device_init(wdg_dev);
    rt_device_control(wdg_dev, RT_DEVICE_CTRL_WDT_KEEPALIVE, NULL);
    rt_thread_idle_sethook(idle_hook);
    
    return 0;
}
INIT_APP_EXPORT(wdt_init);
