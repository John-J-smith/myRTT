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
    int count1 = 0;

    while (1)
    {
        //LOG_I("Thread1 count:%d", ++count1);
        HT_FreeDog();
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
    rt_thread_t thread1_ptr, thread2_ptr, thread3_ptr;

    /* set LED0 pin mode to output
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
        rt_pin_write(LED0_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_pin_write(LED0_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }*/

    thread1_ptr = rt_thread_create("thread1",
                                    thread1_entry, RT_NULL,
                                    2048, 5, 1);

    thread2_ptr = rt_thread_create("thread2",
                                    thread2_entry, RT_NULL,
                                    512, 5, 1);

    thread3_ptr = rt_thread_create("thread3",
                                    thread3_entry, RT_NULL,
                                    512, 5, 1);

    if (thread1_ptr != RT_NULL) rt_thread_startup(thread1_ptr);
    if (thread2_ptr != RT_NULL) rt_thread_startup(thread2_ptr);
    if (thread3_ptr != RT_NULL) rt_thread_startup(thread3_ptr);

    return RT_EOK;
}
