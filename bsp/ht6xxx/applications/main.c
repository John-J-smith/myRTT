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
#include <sal_socket.h>

#ifndef ULOG_USING_SYSLOG
#define LOG_TAG              "main"
#define LOG_LVL              LOG_LVL_DBG
#include <ulog.h>
#else
#include <syslog.h>
#endif /* ULOG_USING_SYSLOG */

void server_thread(void *parameter)
{
    struct sockaddr_in client_addr;
    struct sockaddr_in server_addr;
    int sockfd = -1;

    while(1)
    {
        HT_FreeDog();
        rt_thread_mdelay(500);
    }
}

void led_thread_entry(void* parameter)
{
#define interval 250

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
        rt_pin_write(IO_RUN_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RUN_LED, PIN_HIGH);

        rt_pin_write(IO_TX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_TX_LED, PIN_HIGH);

        rt_pin_write(IO_RX_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RX_LED, PIN_HIGH);

        rt_pin_write(IO_485_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_485_LED, PIN_HIGH);

        rt_pin_write(IO_RELAY_A_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_A_LED, PIN_HIGH);

        rt_pin_write(IO_RELAY_B_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_B_LED, PIN_HIGH);

        rt_pin_write(IO_RELAY_C_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_RELAY_C_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_1_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_1_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_2_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_2_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_3_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_3_LED, PIN_HIGH);

        rt_pin_write(IO_SIG_4_LED, PIN_LOW);
        rt_thread_mdelay(interval);
        rt_pin_write(IO_SIG_4_LED, PIN_HIGH);
    }
}

int main(void)
{
    rt_thread_t led_thread_ptr, server_thread_ptr;
    rt_uint32_t reboot_cnt = 0, len = 0;
    char reboot_cnt_buf[11];

    HT_FreeDog();
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

    server_thread_ptr = rt_thread_create("server",
                                    server_thread, RT_NULL,
                                    2048, 5, 1);

    if (led_thread_ptr != RT_NULL) rt_thread_startup(led_thread_ptr);
    if (server_thread_ptr != RT_NULL) rt_thread_startup(server_thread_ptr);

    return RT_EOK;
}
