/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-11-19     John Shi          first version
 */

#include <board.h>
#include <rtdevice.h>
#include <rthw.h>

#ifdef BSP_USING_WDT

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "drv.wdt"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

static struct rt_watchdog_ops ops;
static rt_watchdog_t watchdog;

static rt_err_t wdt_init(rt_watchdog_t *wdt)
{
    WDT_SetPeriod(WDT_2_SECS);
    WDT_Enable();
    WDT_Clear();

    return RT_EOK;
}

static rt_err_t wdt_control(rt_watchdog_t *wdt, int cmd, void *arg)
{
    switch (cmd)
    {
        /* feed the watchdog */
    case RT_DEVICE_CTRL_WDT_KEEPALIVE:
        WDT_Clear();
        break;
        /* set watchdog timeout */
    case RT_DEVICE_CTRL_WDT_SET_TIMEOUT:
        WDT_SetPeriod((rt_uint32_t)arg);
        break;
    case RT_DEVICE_CTRL_WDT_GET_TIMEOUT:
        break;
        /* enable watchdog */
    case RT_DEVICE_CTRL_WDT_START:
        WDT_Enable();
        break;
        /* disable watchdog */
    case RT_DEVICE_CTRL_WDT_STOP:
        WDT_Disable();
        break;
    default:
        return -RT_ERROR;
    }
    return RT_EOK;
}

int rt_wdt_init(void)
{
    ops.init = &wdt_init;
    ops.control = &wdt_control;
    watchdog.ops = &ops;
    /* register watchdog device */
    if (rt_hw_watchdog_register(&watchdog, "wdt", RT_DEVICE_FLAG_DEACTIVATE, RT_NULL) != RT_EOK)
    {
        LOG_E("wdt device register failed.");
        return -RT_ERROR;
    }
    LOG_D("wdt device register success.");
    return RT_EOK;
}
INIT_BOARD_EXPORT(rt_wdt_init);

#endif /* RT_USING_WDT */
