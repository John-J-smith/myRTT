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

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "main"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */


void thread1_entry(void* parameter)
{
    while(1)
    {
        HD_clrWDT();
        rt_thread_mdelay(500);
    }
}

void thread2_entry(void* parameter)
{
    rt_device_t uart3;

    uart3 = rt_device_find("uart3");
    rt_device_open(uart3, RT_DEVICE_OFLAG_RDWR | RT_DEVICE_FLAG_INT_RX);
    while (1)
    {

        rt_thread_mdelay(100);
    }
}

void thread3_entry(void* parameter)
{
    rt_pin_mode(IO_RUN_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_TX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RX_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_A_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_B_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_RELAY_C_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_1_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_2_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_3_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_SIG_4_LED, PIN_MODE_OUTPUT);
    rt_pin_mode(IO_485_LED, PIN_MODE_OUTPUT);
    while (1)
    {
        rt_pin_write(IO_RUN_LED, PIN_HIGH);
        rt_pin_write(IO_TX_LED, PIN_HIGH);
        rt_pin_write(IO_RX_LED, PIN_HIGH);
        rt_pin_write(IO_RELAY_A_LED, PIN_HIGH);
        rt_pin_write(IO_RELAY_B_LED, PIN_HIGH);
        rt_pin_write(IO_RELAY_C_LED, PIN_HIGH);
        rt_pin_write(IO_SIG_1_LED, PIN_HIGH);
        rt_pin_write(IO_SIG_2_LED, PIN_HIGH);
        rt_pin_write(IO_SIG_3_LED, PIN_HIGH);
        rt_pin_write(IO_SIG_4_LED, PIN_HIGH);
        rt_pin_write(IO_485_LED, PIN_HIGH);
        rt_thread_mdelay(500);

        rt_pin_write(IO_RUN_LED, PIN_LOW);
        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_pin_write(IO_RELAY_A_LED, PIN_LOW);
        rt_pin_write(IO_RELAY_B_LED, PIN_LOW);
        rt_pin_write(IO_RELAY_C_LED, PIN_LOW);
        rt_pin_write(IO_SIG_1_LED, PIN_LOW);
        rt_pin_write(IO_SIG_2_LED, PIN_LOW);
        rt_pin_write(IO_SIG_3_LED, PIN_LOW);
        rt_pin_write(IO_SIG_4_LED, PIN_LOW);
        rt_pin_write(IO_485_LED, PIN_LOW);
        rt_thread_mdelay(500);
    }
}

int main(void)
{
    rt_thread_t led_thread_ptr, server_thread_ptr;
    rt_uint32_t reboot_cnt = 0, len = 0;
    char reboot_cnt_buf[11];

    HD_clrWDT();
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


    server_thread_ptr = rt_thread_create("server",
                                    server_thread, RT_NULL,
                                    256, 5, 1);

    if (server_thread_ptr != RT_NULL) rt_thread_startup(server_thread_ptr);


    return RT_EOK;
}
